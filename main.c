#include <stdio.h>
#include "helper.h"

int main(int argc, char *argv[]) {
    char input[256];
    int choice;

    printf("=== Graphic Calculator ===\n");
    printf("1. Simple Calculator\n");
    printf("2. Differentiate\n");
    printf("3. Show Graphs\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > 3) {
        printf("Invalid choice\n");
        return 1;
    }

    if (!read_input(argc, argv, input, choice)) {
        return 1;
    }
    return 0;
}

