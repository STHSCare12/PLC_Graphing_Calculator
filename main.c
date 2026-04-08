#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "helper.h"
#include "parser.h"
#include "ast.h"
#include "evaluator.h"
#include "dualUtils.h"
#include "graph.h"
#include "tests.h"

/* Used for Mode 1: Simple Calculator */
void simpleCalc(ASTNode* ast, double *result) {
    *result = evaluate_expression(ast);

    /* Free memory */
    freeAST(ast);
}

/* Used for Mode 2: Differentiate */
void differentiation(ASTNode* ast, ASTNode** derivative, double *x, double *y_val, double *dy_val, int file) {
    printf("\ny = ");
    print_expression(ast);
    printf("\n");

    *derivative = differentiate(ast);
    *derivative = simplify(*derivative);

    printf("dy/dx = ");
    print_expression(*derivative);
    printf("\n");

    if (!file) {
        printf("\nEnter value of x: ");
        scanf("%lf", x);
        getchar();

        CURRENT_X = *x;

        *y_val = evaluate_expression(ast);
        *dy_val = evaluate_expression(*derivative);

        printf("\ny(%g) = %f\n", *x, *y_val);
        printf("dy/dx(%g) = %f\n", *x, *dy_val);
    }

    /* Free memory */
    freeAST(ast);
    freeAST(*derivative);
}

void showGraphs(ASTNode* ast) {
    printf("\ny = ");
    print_expression(ast);
    printf("\n");

    plot_graph(ast);

    /* Free memory*/
    freeAST(ast);
}

void suppressPrint() {
    if (freopen("/dev/null", "w", stdout) == NULL) {
        freopen("NUL", "w", stdout);
    }
}

void unsuppressPrint() {
    if (freopen("/dev/tty", "w", stdout) == NULL) {
        freopen("CON", "r", stdout);
    }
}

void printSelectedChoice(int choice) {
    printf("\n-------------------------");   

    switch (choice) {
        case 0:
            printf("\n===      RUN TEST     ===\n");
            break;
        case 1:
            printf("\n=== SIMPLE CALCULATOR ===\n");
            break;
        case 2:
            printf("\n===  DIFFERENTIATION  ===\n");
            break;
        case 3:
            printf("\n===    SHOW GRAPHS    ===\n");
            break;
    }   
    
    printf("-------------------------\n");   
}

void printMenu() {
    printf("\n----------------------------"); 
    printf("\n===  Graphic Calculator  ===\n");
    printf("----------------------------\n"); 
    printf("1. Simple Calculator\n");
    printf("2. Differentiate\n");
    printf("3. Show Graphs\n");
    printf("4. Exit Programme\n\n");
}

int main(int argc, char *argv[]) {
    char input[256];
    char line[128];
    int choice = 0;
    Token tokens[MAX_TOKENS];
    ASTNode* ast;
    double result;

    /* Differentiation */
    ASTNode* derivative;
    double x;
    double y_val;
    double dy_val;

    /* Testing */
    Test tests[MAX_TESTS];
    int success;
    int i;

    while (choice != 4) {
        printMenu();

        while (1) {
            printf("Enter choice: ");
            /* scanf("%d", &choice); */
            /* getchar(); */
            if (!fgets(line, sizeof(line), stdin)) {
                printf("Error reading input\n");
                continue;
            }
            if (strlen(line) == 2) {choice = atoi(line);}
            else {choice = -1;} 
            if (choice < 0 || choice > 4 ) {
                printf("Invalid choice. Please try again.\n");
            }
            else if (choice == 4) {
                printf("Exiting Programme...\n");
                return 0;
            }
            else {
                if ((choice == 0) && (argc > 1)) {printf("Testing is disabled for file mode! Please try again.\n\n"); continue;}
                printSelectedChoice(choice);
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

            if (choice == 0) {continue;}
            while (fgets(input, sizeof(input), file)) {
                char buffer[MAX_INPUT];
                char *equation;

                input[strcspn(input, "\n")] = '\0';
                if (strlen(input) == 0) continue;

                strcpy(buffer, input);
                equation = strtok(buffer, ",");

                while (equation != NULL) {
                    printf("\n-------------------------\n");
                    char trimmed[MAX_INPUT];
                    strncpy(trimmed, equation, MAX_INPUT - 1);
                    trimmed[MAX_INPUT - 1] = '\0';
                    trim(trimmed);
                    if (strlen(trimmed) == 0) {
                        equation = strtok(NULL, ",");
                        continue;
                    }

                    memset(tokens, 0, sizeof(Token) * MAX_TOKENS);
                    if (process_equation(trimmed, choice, tokens) == 1) {
                        equation = strtok(NULL, ",");
                        continue;
                    }

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
                        simpleCalc(ast, &result);
                        printf("\nResult: %f\n", result);
                    }

                    /* Mode 2: Differentiate */
                    if (choice == 2) {
                        differentiation(ast, &derivative, &x, &y_val, &dy_val, 1);
                    }

                    /* Mode 3: Show Graphs */
                    if (choice == 3) {
                        showGraphs(ast);
                    }

                    /*adding this for a seperation line space before next equation*/
                    equation = strtok(NULL, ",");
                }
            }

            fclose(file);
            printf("\n-------------------------\n");
            continue; /* back to menu */
        }

        /* testing */
        if (choice == 0) {

            /* simple calc tests*/
            init_SimpleTests(tests);
            
            for (i=0;i<MAX_TESTS;i++) {
                suppressPrint();
                process_equation(tests[i].eqn, tests[i].type, tokens); 
                unsuppressPrint();  
                ast = buildAST(tokens, choice);

                if (!ast) {
                    printf("Error: Failed to build AST for test %d: %s\n", i, tests[i].eqn);
                    continue;
                }

                simpleCalc(ast, &result);
                
                /* Verify if the calculated result is correct */
                if (verifyTest(tests[i], result, i) == 0) {
                    success += 1;
                }
            }

            printf("\n-------------------------");   
            printf("\n===   TESTS SUMMARY   ===\n");
            printf("-------------------------\n");   
            printf("Passed %d/%d Tests\n", success, MAX_TESTS);
        }
        else {
            /* stdin */
            printf("Enter equation: ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Error reading input\n");
                continue;
            }
            input[strcspn(input, "\n")] = '\0';
            if (process_equation(input, choice, tokens) == 1) {continue;}

            ast = buildAST(tokens, choice);

            if (!ast) {
                printf("Error: Failed to build AST\n");
                continue;
            }

            printf("\n=== AST ===\n");
            printAST(ast, 0);
        }

        /* Mode 1: Simple Calculator */
        if (choice == 1) {
            simpleCalc(ast, &result);
            printf("\nResult: %f\n", result);
        }

        /* Mode 2: Differentiate */
        if (choice == 2) {
            differentiation(ast, &derivative, &x, &y_val, &dy_val, 0);
        }

        /* Mode 3: Show Graphs */
        if (choice == 3) {
            showGraphs(ast);
        }

    }

    return 0;
}

