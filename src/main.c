/* main.c */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "rpn.h"
#include "token_list.h"
#include "tokenizer.h"

int main(void)
{
    printf("ASCII Graph Plotter\nВведите выражение (или 'exit' для выхода):\n");
    while (1) {
        int success = 1;
        char expr[256];
        TokenList *tokens = NULL;
        RPNList *rpn = NULL;
        double dummy;

        printf("> ");
        if (!fgets(expr, sizeof(expr), stdin)) {
            success = 0;
            fprintf(stderr, "Ошибка: не удалось прочитать ввод.\n");
            break;
        }
        expr[strcspn(expr, "\n")] = '\0';
        if (strcmp(expr, "exit") == 0 || strcmp(expr, "quit") == 0) {
            printf("Выход.\n");
            break;
        }
        if (expr[0] == '\0') {
            continue; // пустой ввод — просто ждем следующее выражение
        }
        tokens = tokenize(expr);
        if (!tokens) {
            success = 0;
            fprintf(stderr, "Ошибка: лексический разбор не удался (неизвестный символ, синтаксис или функция).\n");
        }
        if (success) {
            rpn = convert_to_rpn(tokens);
            if (!rpn) {
                success = 0;
                fprintf(stderr, "Ошибка: несбалансированные скобки или синтаксическая ошибка.\n");
            }
        }
        if (success) {
            if (!evaluate_rpn(rpn, 0.0, &dummy)) {
                success = 0;
                fprintf(stderr, "Ошибка: вычисление не удалось (например, деление на 0, некорректная функция или выражение).\n");
            }
        }
        if (success) {
            draw_graph(rpn);
            // Спросить пользователя о сохранении графика
            printf("Сохранить график в файл? (y/n): ");
            char answer[16];
            if (fgets(answer, sizeof(answer), stdin)) {
                if (answer[0] == 'y' || answer[0] == 'Y') {
                    printf("Имя файла (по умолчанию graph.txt): ");
                    char fname[128] = "graph.txt";
                    if (fgets(fname, sizeof(fname), stdin)) {
                        fname[strcspn(fname, "\n")] = '\0';
                        if (fname[0] == '\0') strcpy(fname, "graph.txt");
                    } else {
                        strcpy(fname, "graph.txt");
                    }
                    if (draw_graph_to_file(rpn, fname)) {
                        printf("График сохранён в файл: %s\n", fname);
                    } else {
                        printf("Ошибка при сохранении файла!\n");
                    }
                }
            }
        } else {
            printf("n/a\n");
        }
        if (tokens) {
            free_token_list(tokens);
        }
        if (rpn) {
            free_rpn_list(rpn);
        }
    }
    return 0;
}