#include "stack_token.h"

#include <stdlib.h>

TokenStack *create_token_stack(void)
{
    TokenStack *s = malloc(sizeof(TokenStack));
    if (s != NULL) {
        s->top = -1;
    }
    return s;
}

void free_token_stack(TokenStack *s)
{
    if (s != NULL) {
        free(s);
    }
}

int push_token(TokenStack *s, Token t)
{
    if (s == NULL) {
        return 0;
    }
    int result = 0;
    if (s->top < 127) {
        s->top++;
        s->data[s->top] = t;
        result = 1;
    }
    return result;
}

Token pop_token(TokenStack *s)
{
    Token t = {0};
    if (s != NULL && s->top >= 0) {
        t = s->data[s->top];
        s->top--;
    }
    return t;
}

Token peek_token(TokenStack *s)
{
    Token t = {0};
    if (s != NULL && s->top >= 0) {
        t = s->data[s->top];
    }
    return t;
}

int token_stack_empty(TokenStack *s)
{
    if (s == NULL) {
        return 1;
    }
    return s->top < 0;
}