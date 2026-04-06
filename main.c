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
    ASTNode* ast;
    ASTNode* derivative;
    double x;
    double y_val;
    double dy_val;

    while (choice != 4) {
        printf("=== Graphic Calculator ===\n");
        printf("1. Simple Calculator\n");
        printf("2. Differentiate\n");
        printf("3. Show Graphs\n");
        printf("4. Exit Programme\n");

        while (1) {
            printf("Enter choice: ");
            /* scanf("%d", &choice); */
            /* getchar(); */
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

        /* file mode */
        if (argc > 1) {
            FILE *file = fopen(argv[1], "r");
            if (!file) {
                perror("Error opening file");
                return 1;
            }

            while (fgets(input, sizeof(input), file)) {
                char buffer[MAX_INPUT];
                char *equation;

                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) == 0) continue;

                strcpy(buffer, input);
                equation = strtok(buffer, ",");

                while (equation != NULL) {
                    char trimmed[MAX_INPUT];
                    strncpy(trimmed, equation, MAX_INPUT - 1);
                    trimmed[MAX_INPUT - 1] = '\0';
                    trim(trimmed);
                    if (strlen(trimmed) == 0) {
                        equation = strtok(NULL, ",");
                        continue;
                    }

                    memset(tokens, 0, sizeof(Token) * MAX_TOKENS);
                    process_equation(trimmed, choice, tokens);

                    ast = buildAST(tokens, choice);
                    if (!ast) {
                        printf("Error: Failed to build AST for: %s\n", trimmed);
                        equation = strtok(NULL, ",");
                        continue;
                    }

                    printf("\n=== AST ===\n");
                    printAST(ast, 0);

                    /* Mode 1: Simple Calculator */
                    if (choice == 1) {
                        double result = evaluate_expression(ast);
                        printf("Result: %f\n\n", result);
                        /* Free memory*/
                        freeAST(ast);
                    }

                    /* Mode 2: Differentiate */
                    if (choice == 2) {
                        printf("y = ");
                        print_expression(ast);
                        printf("\n");
                        derivative = differentiate(ast);
                        derivative = simplify(derivative);

                        printf("dy/dx = ");
                        print_expression(derivative);
                        printf("\n");

                        /*removed enter value option for file mode*/
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
                        printf("\n");
                    }

                    /*adding this for a seperation line space before next equation*/
                    printf("---\n");
                    equation = strtok(NULL, ",");
                }
            }

            fclose(file);
            continue; /* back to menu */
        }

        /* stdin */
        printf("Enter equation: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        process_input(input, choice, tokens);

        ast = buildAST(tokens, choice);
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
            derivative = differentiate(ast);
            derivative = simplify(derivative);

            printf("dy/dx = ");
            print_expression(derivative);
            printf("\n");

            printf("Enter value of x: ");
            scanf("%lf", &x);
            getchar();

            CURRENT_X = x;

            y_val = evaluate_expression(ast);
            dy_val = evaluate_expression(derivative);

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

