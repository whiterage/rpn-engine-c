#include "tokenizer.h"

#include <stdlib.h>
#include <string.h>

#include "token_list.h"

// Проверяет, является ли символ цифрой
int is_digit(char c)
{
    return (c >= '0' && c <= '9') ? 1 : 0;
}

// Проверяет, начинается ли строка с именем функции (sin, cos, ...)
// Если да — возвращает длину имени через len
int is_function(const char *str, int *len)
{
    const char *funcs[] = {"sin", "cos", "tan", "ctg", "sqrt", "ln"};
    int count = sizeof(funcs) / sizeof(funcs[0]);
    int i = 0;
    int result = 0;
    *len = 0;

    while (i < count && result == 0) {
        int l = strlen(funcs[i]);
        if (strncmp(str, funcs[i], l) == 0) {
            *len = l;
            result = 1;
        }
        i++;
    }

    return result;
}

// Парсит число из выражения, добавляет токен в список
int parse_number(const char *expr, int *i, TokenList *list)
{
    char buffer[64];
    int j = 0;
    int start = *i;
    int result = 0;

    while ((is_digit(expr[*i]) || expr[*i] == '.') && j < 63) {
        buffer[j++] = expr[*i];
        (*i)++;
    }

    if (j > 0) {
        buffer[j] = '\0';
        Token token = {0};
        token.type = TOKEN_NUMBER;
        strncpy(token.value, buffer, sizeof(token.value) - 1);
        token.value[sizeof(token.value) - 1] = '\0';
        add_token(list, token);
        result = 1;
    } else {
        *i = start;
    }

    return result;
}

// Парсит функцию (sin, cos, ...) или переменную (x)
int parse_function_or_variable(const char *expr, int *i, TokenList *list)
{
    int len = 0;
    int result = 0;

    if (is_function(&expr[*i], &len) == 1) {
        Token token;
        token.type = TOKEN_FUNCTION;
        token.op = '\0';
        strncpy(token.value, &expr[*i], len);
        token.value[len] = '\0';
        *i += len;
        add_token(list, token);
        result = 1;
    } else if (expr[*i] == 'x') {
        Token token = {0};
        token.type = TOKEN_VARIABLE;
        add_token(list, token);
        (*i)++;
        result = 1;
    }

    return result;
}

// Парсит оператор (+, -, *, /, ^) или скобку, добавляет токен в список
int parse_operator_or_paren(const char *expr, int *i, TokenList *list)
{
    int result = 0;
    char c = expr[*i];
    Token token = {0}; // обнуляем сразу всё

    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
        token.type = TOKEN_OPERATOR;
        // Определяем унарный минус
        if (c == '-') {
            int prev = *i - 1;
            // Унарный минус: в начале строки, после (, после оператора
            int is_unary = 0;
            while (prev >= 0 && expr[prev] == ' ') prev--; // пропускаем пробелы
            if (prev < 0) {
                is_unary = 1;
            } else if (expr[prev] == '(' || expr[prev] == '+' || expr[prev] == '-' || expr[prev] == '*' || expr[prev] == '/' || expr[prev] == '^') {
                is_unary = 1;
            }
            if (is_unary) {
                token.op = '~'; // специальный символ для унарного минуса
            } else {
                token.op = '-';
            }
        } else {
            token.op = c;
        }
        add_token(list, token);
        (*i)++;
        result = 1;
    } else if (c == '(') {
        token.type = TOKEN_LPAREN;
        add_token(list, token);
        (*i)++;
        result = 1;
    } else if (c == ')') {
        token.type = TOKEN_RPAREN;
        add_token(list, token);
        (*i)++;
        result = 1;
    }
    return result; // символ не распознан
}

// Основная функция: разбирает выражение на токены, возвращает список токенов или NULL при ошибке
TokenList *tokenize(const char *expr)
{
    TokenList *list = NULL;
    int i = 0;
    int error = 0;

    if (expr != NULL && expr[0] != '\0') {
        list = create_token_list();
    }
    while (list != NULL && expr[i] != '\0' && error == 0) {
        char c = expr[i];
        if (c == ' ') {
            i++;
        } else {
            int parsed = 0;
            // Сначала пробуем разобрать число
            parsed = parse_number(expr, &i, list);
            if (parsed == 0) {
                // Затем функцию или переменную
                parsed = parse_function_or_variable(expr, &i, list);
            }
            // Если после числа сразу идёт переменная (например, 2x), вставим '*'
            if (expr[i] == 'x') {
                Token mul;
                mul.type = TOKEN_OPERATOR;
                mul.op = '*';
                add_token(list, mul);
            }
            if (parsed == 0) {
                // Затем оператор или скобку
                parsed = parse_operator_or_paren(expr, &i, list);
            }
            if (parsed == 0) {
                // Не удалось разобрать символ — ошибка
                error = 1;
            }
        }
    }
    if (list != NULL && error == 1) {
        free_token_list(list);
        list = NULL;
    }
    return list;
}
