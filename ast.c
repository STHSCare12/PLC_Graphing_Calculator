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
        Token t;
        t.type = TOKEN_END;
        t.value[0] = '\0';
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
/* ASTNode* parseUnaryMinus(); */
ASTNode* parsePower();
ASTNode* parseTerm();
ASTNode* parseExpression();

/* Parse a Factor */
ASTNode* parseFactor() {
    Token t;
    ASTNode* node;
    ASTNode* child;
    t = current();
    if (t.type == TOKEN_END) {return NULL;}

    /* Unary Minus */
    if (t.type == TOKEN_OPERATOR && t.value[0] == '-') {
        next();
        child = parseFactor();
        if (!child) return NULL;
        node = (ASTNode*) malloc(sizeof(ASTNode));
        node->type = 'u';
        node->left = child;
        node->right = NULL;
        return node;
    }

    /* Number */
    if (t.type == TOKEN_NUMBER) {
        node = (ASTNode*) malloc (sizeof(ASTNode));
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

            node = (ASTNode*) malloc(sizeof(ASTNode));
            node->type = 'f';
            node->left = node->right = NULL;
            node->value = 0;
            strncpy(node->func, t.value, sizeof(node->func)-1);
            node->func[sizeof(node->func)-1] = '\0';
            next();
            t = current();
            if (t.type != TOKEN_LPAREN) {
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
                return NULL;
            }
            t = current();
            if (t.type != TOKEN_RPAREN) {
                printf("Error: Expected ')'\n");
                freeAST(node);
                return NULL;
            }
            next();
            return node;
        }

        /* Variable: x */
        if (strcmp(t.value, "x") == 0) {
            if (calcMode == 1) {
                /* printf("Invalid!! Variables not allowed in Simple Calculator mode\n"); */
                return NULL;
            }
            node = (ASTNode*) malloc(sizeof(ASTNode));
            node->type = 'v';
            node->left = node->right = NULL;
            node->value = 0;
            next();
            return node;
        }
        printf("Error: Unknown identifier '%s'\n", t.value);
        return NULL;
    }

    /* Parentheses */
    if (t.type == TOKEN_LPAREN) {
        next();
        node = parseExpression();
        if (!node) return NULL;
        t = current();
        if (t.type != TOKEN_RPAREN) {
            printf("Error: Expected ')'\n");
            freeAST(node);
            return NULL;
        }
        next();
        return node;
    }
    printf("Error: Invalid factor\n");
    return NULL;
}

/* Parse a Unary Minus */
/* ASTNode* parseUnaryMinus() {
    Token t;
    ASTNode* node;
    ASTNode* left;
    t = current();
    if (t.type == TOKEN_OPERATOR && t.value[0] == '-') {
        next();
        left = parseUnaryMinus();
        if (!left) return NULL;
        node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = 'u';
        node->left = left;
        node->right = NULL;
        return node;
    }
    return parsePower();
} */

/* Parse a Power */
ASTNode* parsePower() {
    ASTNode* left;
    ASTNode* right;
    ASTNode* node;
    Token t;
    left = parseFactor();
    t = current();
    while (t.type != TOKEN_END && t.type == TOKEN_OPERATOR && t.value[0] == '^') {
        next();
        right = parsePower();
        node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = '^';
        node->left = left;
        node->right = right;
        node->value = 0;
        left = node;
        t = current();
    }
    return left;
}

/* Parse a Term */
ASTNode* parseTerm() {
    ASTNode* left;
    ASTNode* right;
    ASTNode* node;
    char op;
    Token t;
    left = parsePower();
    t = current();
    while (t.type != TOKEN_END && t.type == TOKEN_OPERATOR && (t.value[0] == '*' || t.value[0] == '/')) {
        op = t.value[0];
        next();
        right = parsePower();
        node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = op;
        node->left = left;
        node->right = right;
        node->value = 0;
        left = node;
        t = current();
    }
    return left;
}

/* Parse an Expression */
ASTNode* parseExpression() {
    ASTNode* left;
    ASTNode* right;
    ASTNode* node;
    char op;
    Token t;
    left = parseTerm();
    t = current();
    while (t.type != TOKEN_END && t.type == TOKEN_OPERATOR && (t.value[0] == '+' || t.value[0] == '-')) {
        op = t.value[0];
        next();
        right = parseTerm();
        node = (ASTNode*) malloc (sizeof(ASTNode));
        node->type = op;
        node->left = left;
        node->right = right;
        node->value = 0;
        left = node;
        t = current();
    }
    return left;
}

/* build AST */
ASTNode* buildAST(Token inputTokens[], int mode) {
    ASTNode* ast;
    Token t;
    tokens = inputTokens;
    pos = 0;
    calcMode = mode;
    ast = parseExpression();
    if (!ast) {
        printf("Error: Could not parse expression\n");
        return NULL;
    }
    t = current();
    if (t.type != TOKEN_END) {
        printf("Error: Unexpected token '%s' at end\n", t.value);
        freeAST(ast);
        return NULL;
    }
    return ast;
}

/* print AST */
void printAST(ASTNode* node, int depth) {
    int i;
    if (!node) return;

    for (i = 0; i < depth; i++) {
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