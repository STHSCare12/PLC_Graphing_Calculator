#include "ast.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Token* tokens;
int pos = 0;
int calcMode = 1; /* 1 = Simple Calculator, 2 = Differentiation, 3 = Graph */

/* Current token */
Token current() {
    if (!tokens || tokens[pos].type == TOKEN_END) {
        Token t = {TOKEN_END, ""};
        return t;
    }
    return tokens[pos];
}

/* Move to the next token */
void next() {
    if (tokens[pos].type != TOKEN_END) {
        pos++;
    }
}

/* Check if a node contains a variable (x) */
int containsVariable(ASTNode* node) {
    if (!node) return 0;
    if (node->type == 'v') {
        printf("DEBUG: found variable node\n");
        return 1;
    }
    return containsVariable(node->left) || containsVariable(node->right);
}

/* Free AST recursively */
void freeAST(ASTNode* node) {
    if (!node) return;
    freeAST(node->left);
    freeAST(node->right);
    free(node);
}

/* Declarations */
ASTNode* parseFactor();
ASTNode* parsePower();
ASTNode* parseTerm();
ASTNode* parseExpression();

/* Parse a Factor */
ASTNode* parseFactor() {
    Token t = current();
    if (t.type == TOKEN_END) {return NULL;}

    /* Number */
    if (t.type == TOKEN_NUMBER) {
        ASTNode* node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = 'n';
        node->value = atof(t.value);
        node->left = node->right = NULL;
        next();
        return node;
    }

    /* Identifier: Variable & Function*/
    if (t.type == TOKEN_IDENTIFIER) {
        /* Function: sin, cos, tan, exp, log */
        if (strcmp(t.value, "sin") == 0 || strcmp(t.value, "cos") == 0 
            || strcmp(t.value, "tan") == 0 || strcmp(t.value, "exp") == 0 
            || strcmp(t.value, "log") == 0) {

            ASTNode* node = (ASTNode*) calloc(1, sizeof(ASTNode));
            node->type = 'f';
            strncpy(node->func, t.value, sizeof(node->func)-1);
            node->func[sizeof(node->func)-1] = '\0';
            next();
            if (current().type != TOKEN_LPAREN) {
                printf("Error: Expected '('\n");
                free(node);
                return NULL;
            }
            next();
            node->left = parseExpression();
            if (!node->left) {
                free(node);
                return NULL;
            }
            /* Simple Calculator: reject if function argument contains variable */
            if (calcMode == 1 && containsVariable(node->left)) {
                printf("Invalid!! Function arguments must be numeric constants in Simple Calculator mode\n");
                freeAST(node->left);
                free(node);
                return NULL;
            }
            if (current().type != TOKEN_RPAREN) {
                printf("Error: Expected ')'\n");
                freeAST(node);
                free(node);
                return NULL;
            }
            next();
            node->right = NULL;
            return node;
        }

        /* Variable: x */
        if (strcmp(t.value, "x") == 0) {
            if (calcMode == 1) {
                // printf("Invalid!! Variables not allowed in Simple Calculator mode\n");
                return NULL;
            }
            ASTNode* node = malloc(sizeof(ASTNode));
            node->type = 'v';
            node->left = node->right = NULL;
            next();
            return node;
        }

    }

    /* Parentheses */
    if (t.type == TOKEN_LPAREN) {
        next();
        ASTNode* node = parseExpression();
        if (!node) return NULL;
        if (current().type != TOKEN_RPAREN) {
            printf("Error: Expected ')'\n");
            freeAST(node->left);
            return NULL;
        }
        next();
        return node;
    }
    printf("Error: Invalid factor (unknown identifier '%s')\n", t.value);
    return NULL;
}

/* Parse a Unary Minus */
ASTNode* parseUnaryMinus() {
    if (current().type == TOKEN_OPERATOR && current().value[0] == '-') {
        next();
        ASTNode* left = parseUnaryMinus();
        if (!left) return NULL;
        ASTNode* node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = 'u';
        node->left = left;
        node->right = NULL;
        return node;
    }
    return parseFactor();
}

/* Parse a Power */
ASTNode* parsePower() {
    ASTNode* left = parseFactor();
    while (current().type != TOKEN_END && current().type == TOKEN_OPERATOR && current().value[0] == '^') {
        next();
        ASTNode* right = parseUnaryMinus();
        ASTNode* node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = '^';
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

/* Parse a Term */
ASTNode* parseTerm() {
    ASTNode* left = parsePower();
    while (current().type != TOKEN_END && current().type == TOKEN_OPERATOR && (current().value[0] == '*' || current().value[0] == '/')) {
        char op = current().value[0];
        next();
        ASTNode* right = parsePower();
        ASTNode* node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = op;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

/* Parse an Expression */
ASTNode* parseExpression() {
    ASTNode* left = parseTerm();
    while (current().type != TOKEN_END && current().type == TOKEN_OPERATOR && (current().value[0] == '+' || current().value[0] == '-')) {
        char op = current().value[0];
        next();
        ASTNode* right = parseTerm();
        ASTNode* node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = op;
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

/* build AST */
ASTNode* buildAST(Token inputTokens[], int mode) {
    tokens = inputTokens;
    pos = 0;
    calcMode = mode;
    ASTNode* ast = parseExpression();
    if (!ast) {
        printf("Error: Could not parse expression\n");
        return NULL;
    }
    if (current().type != TOKEN_END) {
        printf("Error: Unexpected token '%s' at end\n", current().value);
        freeAST(ast);
        return NULL;
    }
    return ast;
}

/* print AST */
void printAST(ASTNode* node, int depth) {
    if (!node) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    switch (node->type) {
        case 'n':
            printf("Number: %f\n", node->value);
            break;
        case 'v':
            printf("Variable: x\n");
            break;
        case 'f':
            printf("Function: %s\n", node->func);
            break;
        case 'u':
            printf("Unary Minus\n");
            break;
        case '+': case '-': case '*': case '/': case '^':
            printf("Operator: %c\n", node->type);
            break;
        default:
            printf("Operator: %c\n", node->type);
    }

    printAST(node->left, depth + 1);
    printAST(node->right, depth + 1);
}