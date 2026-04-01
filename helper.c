#include <stdio.h>
#include <string.h>
#include "helper.h"

/* trim whitespaces at the start and end */
void trim(char *str) {
    int start = 0;
    int end;
    int i, j = 0;
    /* while (isspace(str[start])) start++; */
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') start++;

    end = strlen(str) - 1;
    /* while (end > start && isspace(str[end])) end--; */
    while (end > start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) end--;

    for (i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

/* Check if a character is an alphabet */
int isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void process_equation(char *equation, int mode, Token tokens[]) {
    char safe[MAX_INPUT];
    int i;
    int check_variable = 0;
    strncpy(safe, equation, MAX_INPUT - 1);
    safe[MAX_INPUT - 1] = '\0';
    trim(safe);
    if (strlen(safe) == 0) return;

    printf("Equation: %s\n", safe);
    memset(tokens, 0, sizeof(Token) * MAX_TOKENS);
    tokenize(safe, tokens);
    print_tokens(tokens);

    /* check for variables in Simple Calculator mode */
    for (i = 0; tokens[i].type != TOKEN_END; i++) {
        if (tokens[i].type == TOKEN_IDENTIFIER && strlen(tokens[i].value) == 1 && isAlpha(tokens[i].value[0])) {
            check_variable = 1;
            break;
        }
    }
    if (mode == 1 && check_variable) {
        printf("Invalid!! Variables not allowed in Simple Calculator mode\n");
        return;
    }

    if (mode == 1) {
        printf("[Mode: Simple Calculator]\n");
    }
    else if (mode == 2) {
        printf("[Mode: Differentiate]\n");
    }
    else if (mode == 3) {
        printf("[Mode: Graph]\n");
    }
}

void process_input(char *input, int mode, Token tokens[]) {
    /* Token tokens[MAX_TOKENS]; */
    /* int i; */
    /* split by ',' & gets the first chunk */
    /* char *token = strtok(input, ","); */

    // while (token != NULL) {
    //     trim(token);

    //     if (strlen(token) > 0) {
    //         printf("Equation: %s\n", token);
    //         tokenize(token, tokens);
    //         print_tokens(tokens);
        
    //     }

    //     int check_variable= 0;
    //     for (i = 0; tokens[i].type != TOKEN_END; i++){
    //         if (tokens[i].type == TOKEN_IDENTIFIER) {
    //                 /* Only single letter are considered as a variable */
    //                 if (strlen(tokens[i].value) == 1 && isAlpha(tokens[i].value[0])) {
    //                     check_variable = 1;
    //                     break;
    //                 }
    //             } 
    //     }

    //     if (mode == 1 && check_variable) {
    //         printf("Invalid!! Variables not allowed in Simple Calculator mode\n");
    //         token = strtok(NULL, ",");
    //         continue;  /* skip this expression, move on*/
    //     }

        
    //     if (mode == 1) {
    //         printf("[Mode: Simple Calculator]\n");
    //     }
    //     else if (mode == 2) {
    //         printf("[Mode: Differentiate]\n");
    //     }
    //     else if (mode == 3) {
    //         printf("[Mode: Graph]\n");
    //     }

    //     /*next token after that (finds the next ',' )*/
    //     token = strtok(NULL, ",");
    // }
    char buffer[MAX_INPUT];
    strcpy(buffer, input); /* copy input so strtok doesn't modify original */

    char *equation = strtok(buffer, ",");
    while (equation != NULL) {
        char trimmed[MAX_INPUT];
        strncpy(trimmed, equation, MAX_INPUT - 1);
        trimmed[MAX_INPUT - 1] = '\0';
        trim(trimmed);
        if (strlen(trimmed) > 0) {
            memset(tokens, 0, sizeof(Token) * MAX_TOKENS);
            process_equation(trimmed, mode, tokens);
        }
        equation = strtok(NULL, ",");
    }
}

int read_input(int argc, char *argv[], char *buffer, int mode, Token tokens[]) {
    
    /*if file provided, read file -> loop thru each line and process each line*/
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 0;
        }

        while (fgets(buffer, MAX_INPUT, file)) {
            buffer[strcspn(buffer, "\n")] = '\0';

            process_input(buffer, mode, tokens);   
        }
        fclose(file);
    }

    else {
        /* input is stdin */
        printf("Enter equation: ");
        
        if (fgets(buffer, MAX_INPUT, stdin) == NULL) {
            printf("Error reading input\n");
            return 0;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        process_input(buffer, mode, tokens); 
    }
    return 1;
}


void print_tokens(Token tokens[]) {
    int i = 0;
    while (tokens[i].type != TOKEN_END) {
        printf("Token: %s Type: %d\n", tokens[i].value, tokens[i].type);
        i++;
    }
}
