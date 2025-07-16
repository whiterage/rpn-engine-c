#include "stack_double.h"

#include <stdlib.h>

DoubleStack *create_double_stack(void)
{
    DoubleStack *s = malloc(sizeof(DoubleStack));
    if (s != NULL) {
        s->top = -1;
    }
    return s;
}

void free_double_stack(DoubleStack *s)
{
    if (s != NULL) {
        free(s);
    }
}

int push_double(DoubleStack *s, double value)
{
    if (s == NULL) {
        return 0;
    }
    int result = 0;
    if (s->top < 127) {
        s->top++;
        s->data[s->top] = value;
        result = 1;
    }
    return result;
}

int pop_double(DoubleStack *s, double *value)
{
    if (s == NULL || value == NULL) {
        return 0;
    }
    int result = 0;
    if (s->top >= 0) {
        *value = s->data[s->top];
        s->top--;
        result = 1;
    }
    return result;
}

int double_stack_empty(DoubleStack *s)
{
    if (s == NULL) {
        return 1;
    }
    return s->top < 0;
}