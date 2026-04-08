#include <stdio.h>

void suppressPrint(void) {
    if (freopen("/dev/null", "w", stdout) == NULL) {
        freopen("NUL", "w", stdout);
    }
}

void unsuppressPrint(void) {
    if (freopen("/dev/tty", "w", stdout) == NULL) {
        freopen("CON", "w", stdout);
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

void printMenu(void) {
    printf("\n----------------------------"); 
    printf("\n===  Graphic Calculator  ===\n");
    printf("----------------------------\n"); 
    printf("1. Simple Calculator\n");
    printf("2. Differentiate\n");
    printf("3. Show Graphs\n");
    printf("4. Exit Programme\n\n");
}

void printResult(double result) {
    char buffer[256];
    int length;

    length = sprintf(buffer, "%f", result); 

    if (length > 23) {
        /* prints in scientific notation due to limited printing precision */
        printf("%g", result);
    }
    else {
        /* prints normally */
        printf("%f", result);
    }
}
