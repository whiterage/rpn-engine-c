#include <math.h>
#include <stdio.h>
#include <string.h>

#include "rpn.h"

#define HEIGHT 25
#define WIDTH 80

// Строит график функции (выражение в ОПН) в виде ASCII-арт в терминале
void draw_graph(const RPNList *rpn)
{
    char screen[HEIGHT][WIDTH];
    double values[WIDTH];
    double x_start = 0.0;
    double x_end = 4.0 * M_PI;
    double step = (x_end - x_start) / (WIDTH - 1);

    // Заполняем экран точками
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j] = '.';
        }
    }

    // Вычисляем значения функции для каждого столбца (x)
    for (int col = 0; col < WIDTH; col++) {
        double x = x_start + col * step;
        double y = 0.0;
        // Если вычисление успешно и y в диапазоне [-1, 1] — сохраняем
        if (evaluate_rpn(rpn, x, &y) && y >= -1.0 && y <= 1.0 && !isnan(y)) {
            values[col] = y;
        } else {
            values[col] = NAN;
        }
    }

    // Отображаем точки на экране (центр по y = 0)
    for (int col = 0; col < WIDTH; col++) {
        if (!isnan(values[col])) {
            int row = (int)(12.0 - values[col] * 12.0 + 0.5);
            if (row >= 0 && row < HEIGHT) {
                screen[row][col] = '*';
            }
        }
    }

    // Печатаем экран построчно
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

// Строит график и сохраняет его в текстовый файл
int draw_graph_to_file(const RPNList *rpn, const char *filename)
{
    char screen[HEIGHT][WIDTH];
    double values[WIDTH];
    double x_start = 0.0;
    double x_end = 4.0 * M_PI;
    double step = (x_end - x_start) / (WIDTH - 1);
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j] = '.';
        }
    }
    for (int col = 0; col < WIDTH; col++) {
        double x = x_start + col * step;
        double y = 0.0;
        if (evaluate_rpn(rpn, x, &y) && y >= -1.0 && y <= 1.0 && !isnan(y)) {
            values[col] = y;
        } else {
            values[col] = NAN;
        }
    }
    for (int col = 0; col < WIDTH; col++) {
        if (!isnan(values[col])) {
            int row = (int)(12.0 - values[col] * 12.0 + 0.5);
            if (row >= 0 && row < HEIGHT) {
                screen[row][col] = '*';
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fputc(screen[i][j], f);
        }
        fputc('\n', f);
    }
    fclose(f);
    return 1;
}