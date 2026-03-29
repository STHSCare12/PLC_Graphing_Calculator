#include <stdio.h>
#include <string.h>
#include "helper.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    char input[256];
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
            scanf("%d", &choice);
            getchar();
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

        if (!parse(tokens, choice)) {
            return 1;
        }
    }

    return 0;
}

