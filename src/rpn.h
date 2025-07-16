// rpn.h — работа с обратной польской нотацией (ОПН)
#ifndef RPN_H
#define RPN_H

#include "token_list.h"

// Список токенов в ОПН (обратная польская нотация)
typedef struct {
    Token *tokens;     // Массив токенов в ОПН
    int size;          // Количество токенов
    int capacity;      // Ёмкость массива
} RPNList;

// Создаёт новый пустой список ОПН
RPNList *create_rpn_list(void);
// Освобождает память, занятую списком ОПН
void free_rpn_list(RPNList *list);

// Переводит список токенов из инфиксной записи в ОПН (алгоритм сортировочной станции)
RPNList *convert_to_rpn(const TokenList *input);

// Вычисляет значение выражения в ОПН для заданного x
// Возвращает 1 — если успешно, 0 — если ошибка
int evaluate_rpn(const RPNList *rpn, double x, double *result);

// Печатает список ОПН (для отладки)
void print_rpn_list(const RPNList *list);

#endif  // RPN_H