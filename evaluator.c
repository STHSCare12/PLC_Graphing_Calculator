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