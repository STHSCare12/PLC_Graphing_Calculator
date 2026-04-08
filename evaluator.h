#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "helper.h"
#include "ast.h"
#include "dualUtils.h"

double degree_to_radian(double degree);

double evaluate_expression(ASTNode* node);

ASTNode* copy_ast(ASTNode* node);

ASTNode* create_node(char type, double value, const char* func, ASTNode* left, ASTNode* right);

ASTNode* differentiate(ASTNode* node);

Dual evaluate_expression_dual(ASTNode* node, double x);

ASTNode* simplify(ASTNode* node);

void print_expression(ASTNode* node);

extern double CURRENT_X;

#endif
