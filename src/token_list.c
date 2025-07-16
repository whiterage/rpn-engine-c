#include "token_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TokenList *create_token_list(void)
{
    TokenList *list = malloc(sizeof(TokenList));
    if (list != NULL) {
        list->size = 0;
        list->capacity = 8;
        list->tokens = malloc(list->capacity * sizeof(Token));
        if (list->tokens == NULL) {
            free(list);
            list = NULL;
        }
    }
    return list;
}

void free_token_list(TokenList *list)
{
    if (list != NULL) {
        free(list->tokens);
        free(list);
    }
}

void add_token(TokenList *list, Token token)
{
    if (list == NULL) {
        return;
    }
    if (list->size >= list->capacity) {
        int new_capacity = list->capacity * 2;
        Token *new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (new_tokens == NULL) {
            // Не удалось увеличить массив — не добавляем токен
            return;
        }
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }

    Token new_token = {0};
    new_token.type = token.type;

    if (token.type == TOKEN_NUMBER || token.type == TOKEN_FUNCTION) {
        strncpy(new_token.value, token.value, sizeof(new_token.value) - 1);
        new_token.value[sizeof(new_token.value) - 1] = '\0';
    }

    if (token.type == TOKEN_OPERATOR) {
        new_token.op = token.op;
    }

    list->tokens[list->size++] = new_token;
}