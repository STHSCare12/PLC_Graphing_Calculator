#ifndef EVALUATOR_H
#define EVALUATOR_H
#include "helper.h"
#include "ast.h"
#include "dualUtils.h"

double evaluate_expression(ASTNode* node);

Dual evaluate_expression_dual(ASTNode* node, double var_value);

#endif