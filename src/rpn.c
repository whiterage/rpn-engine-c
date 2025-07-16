#include "rpn.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "stack_double.h"
#include "stack_token.h"

// Создаёт новый пустой список ОПН
RPNList *create_rpn_list(void)
{
    RPNList *list = malloc(sizeof(RPNList));
    if (list != NULL) {
        list->tokens = malloc(sizeof(Token) * 128);
        list->size = 0;
        list->capacity = 128;
        if (list->tokens == NULL) {
            free(list);
            list = NULL;
        }
    }
    return list;
}

// Освобождает память, занятую списком ОПН
void free_rpn_list(RPNList *list)
{
    if (list != NULL) {
        if (list->tokens != NULL) {
            free(list->tokens);
        }
        free(list);
    }
}

// Возвращает приоритет оператора (чем больше — тем выше приоритет)
int get_precedence(char op)
{
    int result = 0;
    if (op == '~') result = 3; // унарный минус — самый высокий приоритет
    else if (op == '*' || op == '/') result = 2;
    else if (op == '+' || op == '-') result = 1;
    return result;
}

// Проверяет, является ли оператор левоассоциативным (все — да)
int is_left_associative(char op)
{
    (void)op;
    return 1;
}

// Обрабатывает оператор: снимает со стека операторы с большим/равным приоритетом
static void handle_operator(TokenStack *stack, RPNList *output, Token t)
{
    if (stack == NULL || output == NULL) {
        return;
    }
    while (!token_stack_empty(stack)) {
        Token top = peek_token(stack);
        int prior = get_precedence(top.op) > get_precedence(t.op);
        int same = get_precedence(top.op) == get_precedence(t.op);
        int assoc = is_left_associative(t.op);
        if (!(top.type == TOKEN_FUNCTION || (top.type == TOKEN_OPERATOR && (prior || (same && assoc))))) {
            break;
        }
        output->tokens[output->size++] = pop_token(stack);
    }
    push_token(stack, t);
}

// Обрабатывает правую скобку: снимает операторы до левой скобки
static void handle_rparen(TokenStack *stack, RPNList *output, int *ok)
{
    if (stack == NULL || output == NULL || ok == NULL) {
        return;
    }
    int found = 0;
    while (!token_stack_empty(stack)) {
        Token top = pop_token(stack);
        if (top.type == TOKEN_LPAREN) {
            found = 1;
            break;
        }
        output->tokens[output->size++] = top;
    }
    if (!found) {
        free_rpn_list(output);
        *ok = 0;
    }
    // Если после скобки функция — тоже добавить в выход
    if (*ok && !token_stack_empty(stack) && peek_token(stack).type == TOKEN_FUNCTION) {
        output->tokens[output->size++] = pop_token(stack);
    }
}

// Снимает оставшиеся операторы со стека в выход
static void finalize_stack(TokenStack *stack, RPNList *output, int *ok)
{
    if (stack == NULL || output == NULL || ok == NULL) {
        return;
    }
    while (!token_stack_empty(stack)) {
        Token top = pop_token(stack);
        if (top.type == TOKEN_LPAREN || top.type == TOKEN_RPAREN) {
            free_rpn_list(output);
            *ok = 0;
            break;
        }
        output->tokens[output->size++] = top;
    }
}

// Переводит список токенов из инфиксной записи в ОПН (алгоритм сортировочной станции)
RPNList *convert_to_rpn(const TokenList *input)
{
    RPNList *output = create_rpn_list();
    TokenStack *stack = create_token_stack();
    int ok = (output != NULL && stack != NULL && input != NULL);
    int i = 0;

    if (!ok) {
        if (output) free_rpn_list(output);
        if (stack) free_token_stack(stack);
        return NULL;
    }

    while (ok && i < input->size) {
        Token t = input->tokens[i];
        if (t.type == TOKEN_NUMBER || t.type == TOKEN_VARIABLE) {
            output->tokens[output->size++] = t;
        } else if (t.type == TOKEN_FUNCTION) {
            push_token(stack, t);
        } else if (t.type == TOKEN_OPERATOR) {
            handle_operator(stack, output, t);
        } else if (t.type == TOKEN_LPAREN) {
            push_token(stack, t);
        } else if (t.type == TOKEN_RPAREN) {
            handle_rparen(stack, output, &ok);
        }
        i++;
    }

    if (ok) {
        finalize_stack(stack, output, &ok);
    }
    free_token_stack(stack);
    return ok ? output : NULL;
}

// Применяет оператор к двум числам (a op b)
static int apply_operator(Token t, double a, double b, double *res)
{
    if (res == NULL) {
        return 0;
    }
    int result = 1;
    if (t.op == '+')
        *res = a + b;
    else if (t.op == '-')
        *res = a - b;
    else if (t.op == '*')
        *res = a * b;
    else if (t.op == '/')
        *res = (fabs(b) > 1e-6) ? a / b : NAN;
    else
        result = 0;
    return result;
}

// Применяет функцию к аргументу (sin, cos, ...)
static int apply_function(Token t, double arg, double *res)
{
    if (res == NULL) {
        return 0;
    }
    int result = 1;
    if (strcmp(t.value, "sin") == 0)
        *res = sin(arg);
    else if (strcmp(t.value, "cos") == 0)
        *res = cos(arg);
    else if (strcmp(t.value, "tan") == 0)
        *res = fabs(tan(arg)) < 1e6 ? tan(arg) : NAN;
    else if (strcmp(t.value, "ctg") == 0)
        *res = fabs(tan(arg)) > 1e-6 ? 1.0 / tan(arg) : NAN;
    else if (strcmp(t.value, "sqrt") == 0)
        *res = (arg >= 0) ? sqrt(arg) : NAN;
    else if (strcmp(t.value, "ln") == 0)
        *res = (arg > 0) ? log(arg) : NAN;
    else
        result = 0;
    return result;
}

// Обрабатывает один токен в процессе вычисления ОПН
static void process_token(Token t, DoubleStack *stack, double x_value, int *ok)
{
    if (stack == NULL || ok == NULL) {
        return;
    }
    if (t.type == TOKEN_VARIABLE) {
        *ok = push_double(stack, x_value);
    } else if (t.type == TOKEN_NUMBER) {
        double value = atof(t.value);
        if (value == 0.0 && strcmp(t.value, "0") != 0)
            *ok = 0;
        else
            *ok = push_double(stack, value);
    } else if (t.type == TOKEN_OPERATOR) {
        if (t.op == '~') {
            // Унарный минус: снимаем один аргумент
            double a;
            if (double_stack_empty(stack) || !pop_double(stack, &a)) {
                *ok = 0;
            } else {
                *ok = push_double(stack, -a);
            }
        } else {
            double b, a;
            if (double_stack_empty(stack) || !pop_double(stack, &b) || double_stack_empty(stack) ||
                !pop_double(stack, &a)) {
                *ok = 0;
            } else {
                double res;
                *ok = apply_operator(t, a, b, &res);
                if (*ok) *ok = push_double(stack, res);
            }
        }
    } else if (t.type == TOKEN_FUNCTION) {
        double arg;
        if (double_stack_empty(stack) || !pop_double(stack, &arg)) {
            *ok = 0;
        } else {
            double res;
            *ok = apply_function(t, arg, &res);
            if (*ok) *ok = push_double(stack, res);
        }
    }
}

// Вычисляет значение выражения в ОПН для заданного x
// Возвращает 1 — если успешно, 0 — если ошибка (например, деление на 0, некорректная ОПН)
int evaluate_rpn(const RPNList *rpn, double x_value, double *result)
{
    DoubleStack *stack = create_double_stack();
    int ok = (stack != NULL && rpn != NULL);
    int i = 0;

    if (!ok) {
        if (stack) free_double_stack(stack);
        return 0;
    }

    while (ok && i < rpn->size) {
        process_token(rpn->tokens[i], stack, x_value, &ok);
        i++;
    }

    if (ok && !double_stack_empty(stack)) {
        ok = pop_double(stack, result);
        if (!double_stack_empty(stack)) ok = 0;
    } else {
        ok = 0;
    }

    free_double_stack(stack);
    return ok;
}