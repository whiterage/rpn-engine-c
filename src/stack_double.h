// stack_double.h — стек для чисел double (используется при вычислении ОПН)
#ifndef STACK_DOUBLE_H
#define STACK_DOUBLE_H

// Стек для чисел double (фиксированный размер)
typedef struct {
    double data[128];  // Массив чисел
    int top;           // Индекс вершины стека
} DoubleStack;

// Создаёт новый стек double
DoubleStack *create_double_stack(void);
// Освобождает память, занятую стеком
void free_double_stack(DoubleStack *s);
// Кладёт число на вершину стека
int push_double(DoubleStack *s, double value);
// Снимает число с вершины стека
int pop_double(DoubleStack *s, double *value);
// Проверяет, пуст ли стек
int double_stack_empty(DoubleStack *s);

#endif