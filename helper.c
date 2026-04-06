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

    /* Skip LHS in Mode 2: Differentiate */
    if (mode == 2) {
        char *rhs = strchr(safe, '=');
        if (rhs) {
            rhs++; /* move past '=' */
            while (*rhs == ' ') rhs++; /* skip leading spaces */
            strncpy(safe, rhs, MAX_INPUT - 1);
            safe[MAX_INPUT - 1] = '\0';
        }
    }

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

}

void process_input(char *input, int mode, Token tokens[]) {
    char *equation;
    char buffer[MAX_INPUT];
    strcpy(buffer, input); /* copy input so strtok doesn't modify original */

    equation = strtok(buffer, ",");
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


void print_tokens(Token tokens[]) {
    int i = 0;
    while (tokens[i].type != TOKEN_END) {
        printf("Token: %s Type: %d\n", tokens[i].value, tokens[i].type);
        i++;
    }
}
