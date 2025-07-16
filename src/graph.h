// graph.h — построение ASCII-графика функции по ОПН
#ifndef GRAPH_H
#define GRAPH_H

#include "rpn.h"

// Строит график функции (выражение в ОПН) в виде ASCII-арт в терминале
void draw_graph(RPNList* rpn);

// Строит график и сохраняет его в текстовый файл
int draw_graph_to_file(const RPNList* rpn, const char* filename);

#endif