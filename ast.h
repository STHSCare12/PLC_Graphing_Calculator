#ifndef AST_H
#define AST_H
#include "helper.h"

typedef struct ASTNode {
    char type; /* Types: +, -, *, /, ^ (power), n (number), v (variable), f (function), u (unary minus) */
    double value;
    char func[10];

    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode* buildAST(Token tokens[], int mode);
void printAST(ASTNode* node, int depth);

#endif