#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "helper.h"

/* trim whitespaces at the start and end */
void trim(char *str) {
    int start = 0;
    int end;
    int i, j = 0;
    while (isspace(str[start])) start++;

    end = strlen(str) - 1;
    while (end > start && isspace(str[end])) end--;

    for (i = start; i <= end; i++) {
        str[j++] = str[i];
    }
    str[j] = '\0';
}

void process_input(char *input, int mode) {
    Token tokens[MAX_TOKENS];
    int check_variable= 0;
    int i;
    /* split by ',' & gets the first chunk */
    char *token = strtok(input, ",");

    while (token != NULL) {
        trim(token);

        if (strlen(token) > 0) {
            printf("Equation: %s\n", token);
            tokenize(token, tokens);
            print_tokens(tokens);
        
        }


        for (i = 0; tokens[i].type != TOKEN_END; i++){
            if (tokens[i].type == TOKEN_IDENTIFIER) {
                    check_variable = 1;
                    break;
                } 
        }

        if (mode == 1 && check_variable) {
            printf("Invalid!! Variables not allowed in Simple Calculator mode\n");
            token = strtok(NULL, ",");
            continue;  /* skip this expression, move on*/
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

        /*next token after that (finds the next ',' )*/
        token = strtok(NULL, ",");
    }
}

int read_input(int argc, char *argv[], char *buffer, int mode) {
    
    /*if file provided, read file -> loop thru each line and process each line*/
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Error opening file");
            return 0;
        }

        while (fgets(buffer, MAX_INPUT, file)) {
            buffer[strcspn(buffer, "\n")] = '\0';

            process_input(buffer, mode);   
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
        process_input(buffer, mode); 
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
