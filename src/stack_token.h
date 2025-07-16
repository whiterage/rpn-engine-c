// stack_token.h — стек для токенов (используется при переводе в ОПН)
#ifndef STACK_TOKEN_H
#define STACK_TOKEN_H

#include <stdio.h>
#include "token_list.h"

// Стек для токенов (фиксированный размер)
typedef struct {
    Token data[128];   // Массив токенов
    int top;           // Индекс вершины стека
} TokenStack;

// Создаёт новый стек токенов
TokenStack *create_token_stack(void);
// Освобождает память, занятую стеком
void free_token_stack(TokenStack *s);
// Кладёт токен на вершину стека
int push_token(TokenStack *s, Token t);
// Снимает токен с вершины стека
Token pop_token(TokenStack *s);
// Возвращает токен с вершины стека (не снимая)
Token peek_token(TokenStack *s);
// Проверяет, пуст ли стек
int token_stack_empty(TokenStack *s);

#endif