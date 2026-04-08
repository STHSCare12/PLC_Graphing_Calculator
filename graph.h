#ifndef GRAPH_H
#define GRAPH_H

#include "ast.h"

int contains_trig_functions(ASTNode *node);

void find_x_range(ASTNode *node, double *x_min, double *x_max, int *points);

int is_valid_point(double y, double prev_y);

void plot_graph(ASTNode* node);
#endif
