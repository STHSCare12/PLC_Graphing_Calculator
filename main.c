#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "helper.h"
#include "parser.h"
#include "ast.h"
#include "evaluator.h"
#include "dualUtils.h"

int main(int argc, char *argv[]) {
    char input[256];
    char line[128];
    int choice;
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
        }
        /* Mode 2: Differentiate */
        if (choice == 2) {
            char equation[256];
            printf("Enter function (example: y=sin(x)): ");
            if (!fgets(equation, sizeof(equation), stdin)) {
                printf("Error reading equation\n");
                continue;
            }
            equation[strcspn(equation, "\n")] = '\0';
            char *rhs = strchr(equation, '='); /* Extract right-hand side if user types y=... */
            if (rhs) {
                rhs++; /* move past '=' */
                while (*rhs == ' ') rhs++; /* skip spaces */
            } else {
                rhs = equation;
            }

            /* Tokenize the equation */
            memset(tokens, 0, sizeof(tokens));
            process_equation(rhs, choice, tokens);
            ASTNode* ast = buildAST(tokens, choice);
            if (!ast) {
                printf("Error: Failed to build AST\n");
                continue;
            }

            double x_value;
            printf("Enter value of x to evaluate derivative at: ");
            scanf("%lf", &x_value);
            getchar(); /* consume newline */

            Dual result = evaluate_expression_dual(ast, x_value);

            printf("f(x) = %f\n", result.real);
            printf("f'(x) = %f\n", result.dual);
        }
        printf("\n");
    }

    return 0;
}

