// tokenizer.h — разбор строки выражения на токены
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "token_list.h"

// Проверяет, является ли символ цифрой
int is_digit(char c);
// Проверяет, является ли символ буквой (не используется)
int is_alpha(char c);
// Проверяет, начинается ли строка с имени функции (sin, cos, ...), возвращает длину имени
int is_function(const char *str, int *len);
// Парсит число из выражения, добавляет токен в список
int parse_number(const char *expr, int *i, TokenList *list);
// Парсит функцию или переменную из выражения, добавляет токен в список
int parse_function_or_variable(const char *expr, int *i, TokenList *list);
// Парсит оператор или скобку, добавляет токен в список
int parse_operator_or_paren(const char *expr, int *i, TokenList *list);
// Основная функция: разбирает выражение на токены, возвращает список токенов или NULL при ошибке
TokenList *tokenize(const char *expression);

#endif