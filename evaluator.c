#include "evaluator.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#define PI 3.14159265358979323846

double degree_to_radian(double degree) {
    return degree * PI / 180.0;
}

double evaluate_expression(ASTNode* node) {
    if (!node) return 0;
    switch (node->type)
    {
        case 'n':
            return node->value;
        case '+':
            return evaluate_expression(node->left) + evaluate_expression(node->right);
        case '-':
            return evaluate_expression(node->left) - evaluate_expression(node->right);
        case '*':
            return evaluate_expression(node->left) * evaluate_expression(node->right);
        case '/':
            return evaluate_expression(node->left) / evaluate_expression(node->right);
        case '^':
            return pow(evaluate_expression(node->left), evaluate_expression(node->right));
        case 'f':
            if (strcmp(node->func, "sin") == 0) {
                return sin(degree_to_radian(evaluate_expression(node->left)));
            } else if (strcmp(node->func, "cos") == 0) {
                return cos(degree_to_radian(evaluate_expression(node->left)));
            } else if (strcmp(node->func, "tan") == 0) {
                return tan(degree_to_radian(evaluate_expression(node->left)));
            } else if (strcmp(node->func, "exp") == 0) {
                return exp(evaluate_expression(node->left));
            } else if (strcmp(node->func, "log") == 0) {
                return log10(evaluate_expression(node->left));
            }
            break;
        case 'u':
            return -evaluate_expression(node->left);
        default:
            printf("Error: Unknown node type '%c'\n", node->type);
            return 0;
    }
    return 0;
}

Dual evaluate_expression_dual(ASTNode* node, double var_value) {
    if (!node) return make_dual(0.0f, 0.0f);
    switch (node->type)
    {
        case 'n':
            return make_dual(node->value, 0.0f);
        case 'v':
            return make_dual(var_value, 1.0f);
        case '+':
            return DUAL_ADD.apply(evaluate_expression_dual(node->left, var_value), evaluate_expression_dual(node->right, var_value));
        case '-':
            return DUAL_SUB.apply(evaluate_expression_dual(node->left, var_value), evaluate_expression_dual(node->right, var_value));
        case '*':
            return DUAL_MUL.apply(evaluate_expression_dual(node->left, var_value), evaluate_expression_dual(node->right, var_value));
        case '/':
            return DUAL_DIV.apply(evaluate_expression_dual(node->left, var_value), evaluate_expression_dual(node->right, var_value));
        case '^':
            return DUAL_POW.apply(evaluate_expression_dual(node->left, var_value), evaluate_expression_dual(node->right, var_value));
        case 'f':
            if (strcmp(node->func, "sin") == 0) {
                return DUAL_SIN.apply(evaluate_expression_dual(node->left, var_value));
            } else if (strcmp(node->func, "cos") == 0) {
                return DUAL_COS.apply(evaluate_expression_dual(node->left, var_value));
            } else if (strcmp(node->func, "tan") == 0) {
                return DUAL_TAN.apply(evaluate_expression_dual(node->left, var_value));
            } else if (strcmp(node->func, "exp") == 0) {
                return DUAL_EXP.apply(evaluate_expression_dual(node->left, var_value));
            } else if (strcmp(node->func, "log") == 0) {
                return DUAL_LOG.apply(evaluate_expression_dual(node->left, var_value));
            }
            break;
        case 'u':
            Dual arg = evaluate_expression_dual(node->left, var_value);
            return make_dual(-arg.real, -arg.dual);
        default:
            printf("Error: Unknown node type '%c'\n", node->type);
            return make_dual(0.0f, 0.0f);
    }
}