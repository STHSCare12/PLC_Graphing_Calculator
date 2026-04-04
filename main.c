#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "helper.h"
#include "parser.h"
#include "ast.h"
#include "evaluator.h"
#include "dualUtils.h"
#include "graph.h"

int main(int argc, char *argv[]) {
    char input[256];
    char line[128];
    int choice = 0;
    Token tokens[MAX_TOKENS];

    while (choice != 4) {
        printf("=== Graphic Calculator ===\n");
        printf("1. Simple Calculator\n");
        printf("2. Differentiate\n");
        printf("3. Show Graphs\n");
        printf("4. Exit Programme\n");

        while (1) {
            printf("Enter choice: ");
            // scanf("%d", &choice);
            // getchar();
            if (!fgets(line, sizeof(line), stdin)) {
                printf("Error reading input\n");
                continue;
            }
            choice = atoi(line);
            if (choice < 1 || choice > 4 ) {
                printf("Invalid choice. Please try again.\n");
            }
            else if (choice == 4) {
                printf("Exiting Programme...\n");
                return 0;
            }
            else {
                break;
            }
        }

        if (!read_input(argc, argv, input, choice, tokens)) {
            return 1;
        }

        // if (!parse(tokens, choice)) {
        //     return 1;
        // }

        /* Build AST */
        ASTNode* ast = buildAST(tokens, choice);   // build AST from tokens
        if (!ast) {
            printf("Error: Failed to build AST\n");
            continue;
        }

        printf("\n=== AST ===\n");
        printAST(ast, 0);

        /* Mode 1: Simple Calculator */
        if (choice == 1) {
            double result = evaluate_expression(ast);
            printf("Result: %f\n", result);
            /* Free memory*/
            freeAST(ast);
        }

        /* Mode 2: Differentiate */
        if (choice == 2) {
            printf("y = ");
            print_expression(ast);
            printf("\n");
            ASTNode* derivative = differentiate(ast);
            derivative = simplify(derivative);

            printf("dy/dx = ");
            print_expression(derivative);
            printf("\n");

            double x;
            printf("Enter value of x: ");
            scanf("%lf", &x);
            getchar();

            CURRENT_X = x;

            double y_val = evaluate_expression(ast);
            double dy_val = evaluate_expression(derivative);

            printf("y(%g) = %f\n", x, y_val);
            printf("dy/dx(%g) = %f\n", x, dy_val);

            /* Free memory*/
            freeAST(ast);
            freeAST(derivative);
        }

        /* Mode 3: Show Graphs */
        if (choice == 3) {
            printf("y = ");
            print_expression(ast);
            printf("\n");

            plot_graph(ast);

            /* Free memory*/
            freeAST(ast);
        }

        printf("\n");
    }

    return 0;
}

