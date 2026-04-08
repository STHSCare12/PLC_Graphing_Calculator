#include "evaluator.h"
#include "print.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define PI 3.14159265358979323846

double CURRENT_X = 0;

double degree_to_radian(double degree) {
    return degree * PI / 180.0;
}

double evaluate_expression(ASTNode* node) {
    if (!node) return 0;
    switch (node->type)
    {
        case 'n':
            return node->value;
        case 'v':
            return CURRENT_X;
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

/* Copy AST node */
ASTNode* copy_ast(ASTNode* node) {
    ASTNode* n;
    if (!node) return NULL;
    n = malloc(sizeof(ASTNode));
    n->type = node->type;
    n->value = node->value;
    strcpy(n->func, node->func);
    n->left = copy_ast(node->left);
    n->right = copy_ast(node->right);
    return n;
}

/* Create node */
ASTNode* create_node(char type, double value, const char* func, ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value;
    if (func) strcpy(node->func, func);
    else node->func[0] = '\0';
    node->left = left;
    node->right = right;
    return node;
}

/* Differentiation */
/* ASTNode struct: type, value, func, left, right */
ASTNode* differentiate(ASTNode* node) {
    if (!node) return NULL;
    switch (node->type)
    {
        case 'n':
            /* d/dx (constant) = 0 */
            return create_node('n', 0, NULL, NULL, NULL); 
        case 'v':
            /* d/dx (x) = 1 */
            return create_node('n', 1, NULL, NULL, NULL); 
        case '+':
            /* d/dx (f + g) = df/dx + dg/dx */
            return create_node('+', 0, NULL, differentiate(node->left), differentiate(node->right)); 
        case '-':
            /* d/dx (f - g) = df/dx - dg/dx */
            return create_node('-', 0, NULL, differentiate(node->left), differentiate(node->right)); 
        case '*':
            /* d/dx (f * g) = df/dx * g + f * dg/dx */
            return create_node('+', 0, NULL, create_node('*', 0, NULL, differentiate(node->left), copy_ast(node->right)), create_node('*', 0, NULL, copy_ast(node->left), differentiate(node->right))); 
        case '/':
            /* d/dx (f / g) = (df/dx * g - f * dg/dx) / g^2 */
            return create_node('/', 0, NULL, create_node('-', 0, NULL, create_node('*', 0, NULL, differentiate(node->left), copy_ast(node->right)), create_node('*', 0, NULL, copy_ast(node->left), differentiate(node->right))), create_node('^', 0, NULL, copy_ast(node->right), create_node('n', 2, NULL, NULL, NULL))); 
        case '^':
            if (node->right->type == 'n') {
                /* d/dx (f(x)^c) = c * f(x)^(c-1) * df/dx */
                return create_node('*', 0, NULL, create_node('*', 0, NULL, create_node('n', node->right->value, NULL, NULL, NULL), create_node('^', 0, NULL, copy_ast(node->left), create_node('n', node->right->value - 1, NULL, NULL, NULL))), differentiate(node->left)); 
            } else {
                /* d/dx (f^g) = f^g * (g' * ln(f) + g * f'/f) */
                return create_node('*', 0, NULL, create_node('^', 0, NULL, copy_ast(node->left), copy_ast(node->right)), create_node('+', 0, NULL, create_node('*', 0, NULL, differentiate(node->right), create_node('f', 0, "log", copy_ast(node->left), NULL)), create_node('*', 0, NULL, copy_ast(node->right), create_node('/', 0, NULL, differentiate(node->left), copy_ast(node->left)))));
            }
        case 'f':
            if (strcmp(node->func, "sin") == 0) {
                /* d/dx (sin(x)) = cos(x) * df/dx */
                return create_node('*', 0, NULL, create_node('f', 0, "cos", copy_ast(node->left), NULL), differentiate(node->left)); 
            } else if (strcmp(node->func, "cos") == 0) {
                /* d/dx (cos(x)) = -sin(x) * df/dx */
                return create_node('*', 0, NULL, create_node('u', 0, NULL, create_node('f', 0, "sin", copy_ast(node->left), NULL), NULL), differentiate(node->left)); 
            } else if (strcmp(node->func, "tan") == 0) {
                /* d/dx (tan(x)) = sec^2(x) * df/dx */
                return create_node('*', 0, NULL, create_node('/', 0, NULL, create_node('n', 1, NULL, NULL, NULL), create_node('^', 0, NULL, create_node('f', 0, "cos", copy_ast(node->left), NULL), create_node('n', 2, NULL, NULL, NULL))), differentiate(node->left)); 
            } else if (strcmp(node->func, "exp") == 0) {
                /* d/dx (exp(x)) = exp(x) * df/dx */
                return create_node('*', 0, NULL, create_node('f', 0, "exp", copy_ast(node->left), NULL), differentiate(node->left)); 
            } else if (strcmp(node->func, "log") == 0) {
                /* d/dx log10(x) = (1 / (x * ln(10))) * dx/dx */
                return create_node('*', 0, NULL, create_node('/', 0, NULL, create_node('n', 1.0, NULL, NULL, NULL), create_node('*', 0, NULL, copy_ast(node->left), create_node('n', 2.302585, NULL, NULL, NULL))), differentiate(node->left));
            }
        case 'u':
            /* d/dx (-f) = -df/dx */
            return create_node('u', 0, NULL, differentiate(node->left), NULL);
        default:
            printf("Error: Unknown node type '%c'\n", node->type);
            break;
    }
    return NULL;
}

Dual evaluate_expression_dual(ASTNode* node, double x) {
    Dual a;
    if (!node) return make_dual(0, 0);
    switch (node->type)
    {
        case 'n':
            return make_dual(node->value, 0);
        case 'v':
            return make_dual(x, 1);
        case '+':
            return dual_add(evaluate_expression_dual(node->left, x), evaluate_expression_dual(node->right, x));
        case '-':
            return dual_sub(evaluate_expression_dual(node->left, x), evaluate_expression_dual(node->right, x));
        case '*':
            return dual_mul(evaluate_expression_dual(node->left, x), evaluate_expression_dual(node->right, x));
        case '/':
            return dual_div(evaluate_expression_dual(node->left, x), evaluate_expression_dual(node->right, x));
        case '^':
            return dual_pow(evaluate_expression_dual(node->left, x), evaluate_expression_dual(node->right, x));
        case 'f':
            if (strcmp(node->func, "sin") == 0) {
                return dual_sin(evaluate_expression_dual(node->left, x));
            } else if (strcmp(node->func, "cos") == 0) {
                return dual_cos(evaluate_expression_dual(node->left, x));
            } else if (strcmp(node->func, "tan") == 0) {
                return dual_tan(evaluate_expression_dual(node->left, x));
            } else if (strcmp(node->func, "exp") == 0) {
                return dual_exp(evaluate_expression_dual(node->left, x));
            } else if (strcmp(node->func, "log") == 0) {
                return dual_log(evaluate_expression_dual(node->left, x));
            }
            break;
        case 'u':
            a = evaluate_expression_dual(node->left, x);
            return make_dual(-a.real, -a.dual);
        default:
            printf("Error: Unknown node type '%c'\n", node->type);
            return make_dual(0, 0);
    }
    return make_dual(0, 0);
}

/* Simplify Expression */
ASTNode* simplify(ASTNode* node) {
    if (!node) return NULL;

    node->left = simplify(node->left);
    node->right = simplify(node->right);

    /* Handle constants for +, -, *, / */
    if (node->type == '+') {
        if (node->left && node->left->type == 'n' && node->left->value == 0) return node->right;
        if (node->right && node->right->type == 'n' && node->right->value == 0) return node->left;
        if (node->left && node->left->type == 'n' && node->right && node->right->type == 'n') {
            node->type = 'n';
            node->value = node->left->value + node->right->value;
            free(node->left); free(node->right);
            node->left = node->right = NULL;
            return node;
        }
    }
    if (node->type == '-') {
        if (node->right && node->right->type == 'n' && node->right->value == 0) return node->left;
        if (node->left && node->left->type == 'n' && node->right && node->right->type == 'n') {
            node->type = 'n';
            node->value = node->left->value - node->right->value;
            free(node->left); free(node->right);
            node->left = node->right = NULL;
            return node;
        }
    }
    if (node->type == '*') {
        if ((node->left && node->left->type == 'n' && node->left->value == 0) ||
            (node->right && node->right->type == 'n' && node->right->value == 0)) {
            free(node->left); free(node->right);
            node->type = 'n'; node->value = 0;
            node->left = node->right = NULL;
            return node;
        }
        if (node->left && node->left->type == 'n' && node->left->value == 1) return node->right;
        if (node->right && node->right->type == 'n' && node->right->value == 1) return node->left;

        /* Multiply two numeric constants*/
        if (node->left && node->left->type == 'n' && node->right && node->right->type == 'n') {
            node->type = 'n';
            node->value = node->left->value * node->right->value;
            free(node->left); free(node->right);
            node->left = node->right = NULL;
            return node;
        }
    }

    if (node->type == '/') {
        if (node->left && node->left->type == 'n' && node->left->value == 0) {
            free(node->left); free(node->right);
            node->type = 'n'; node->value = 0;
            node->left = node->right = NULL;
            return node;
        }
        if (node->right && node->right->type == 'n' && node->right->value == 1) return node->left;

        /* Divide two numeric constants */
        if (node->left && node->left->type == 'n' && node->right && node->right->type == 'n') {
            node->type = 'n';
            node->value = node->left->value / node->right->value;
            free(node->left); free(node->right);
            node->left = node->right = NULL;
            return node;
        }
    }

    if (node->type == '^') {
        if (node->right && node->right->type == 'n' && node->right->value == 0) {
            free(node->left); free(node->right);
            node->type = 'n'; node->value = 1;
            node->left = node->right = NULL;
            return node;
        }
        if (node->right && node->right->type == 'n' && node->right->value == 1) return node->left;
        if (node->left && node->left->type == 'n' && node->right && node->right->type == 'n') {
            node->type = 'n';
            node->value = pow(node->left->value, node->right->value);
            free(node->left); free(node->right);
            node->left = node->right = NULL;
            return node;
        }
    }

    /* Unary Minus */
    if (node->type == 'u' && node->left && node->left->type == 'n') {
        node->type = 'n';
        node->value = -node->left->value;
        free(node->left);
        node->left = node->right = NULL;
    }

    return node;
}

/* Print Expression */
void print_expression(ASTNode* node) {
    if (!node) return;
    switch (node->type)
    {
        case 'n':
            printResult(node->value);
            break;
        case 'v':
            printf("x");
            break;
        case '+': case '-': case '*': case '/': case '^':
            printf("(");
            print_expression(node->left);
            printf(" %c ", node->type);
            print_expression(node->right);
            printf(")");
            break;
        case 'f':
            printf("%s(", node->func);
            print_expression(node->left);
            printf(")");
            break;
        case 'u':
            printf("-");
            print_expression(node->left);
            break;
        default:
            printf("Unknown node type '%c'", node->type);
    }
}