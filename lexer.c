#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "helper.h"

/*checks if math operator*/
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int tokenize(const char *input, Token tokens[]) {
    int i = 0;
    int pos = 0;

    while (input[i] != '\0') {
        if (isspace(input[i])) {
            i++; /*skip spaces*/
            continue;
        }

        if (isdigit(input[i]) || input[i] == '.') { /*if its a number or decimal */
            int j = 0;
            while (isdigit(input[i]) || input[i] == '.') {
                // tokens[pos].value[j++] = input[i++];
                if (j < 31) tokens[pos].value[j++] = input[i];
                i++;
            }
            tokens[pos].value[j] = '\0';
            tokens[pos].type = TOKEN_NUMBER; /*store the value and type*/
            pos++;
            continue;
        }

        /*alphabetic letter-> either variable or a function -> identifier*/
        if (isalpha(input[i])) {
            int j = 0;
            while (isalnum(input[i])) { /*x1 is counted*/
                // tokens[pos].value[j++] = input[i++];
                if (j < 31) tokens[pos].value[j++] = input[i];
                i++;
            }
            tokens[pos].value[j] = '\0';
            tokens[pos].type = TOKEN_IDENTIFIER;
            pos++;
            continue;
        }

        /*operator -> token_operator*/
        if (is_operator(input[i])) {
            tokens[pos].type = TOKEN_OPERATOR;
            tokens[pos].value[0] = input[i];
            tokens[pos].value[1] = '\0';
            pos++; i++;
            continue;
        }

        /*left paranthesis -> token_lparan*/
        if (input[i] == '(') {
            tokens[pos].type = TOKEN_LPAREN;
            tokens[pos].value[0] = input[i];
            tokens[pos].value[1] = '\0';
            pos++; i++;
            continue;
        }

        /*right paranthesis -> token_rparan*/
        if (input[i] == ')') {
            tokens[pos].type = TOKEN_RPAREN;
            tokens[pos].value[0] = input[i];
            tokens[pos].value[1] = '\0';
            pos++; i++;
            continue;
        }

        /*comma -> token_comma*/
        if (input[i] == ',') {
            tokens[pos].type = TOKEN_COMMA;
            tokens[pos].value[0] = input[i];
            tokens[pos].value[1] = '\0';
            pos++; i++;
            continue;
        }

        /*else -> token_invalid*/
        tokens[pos].type = TOKEN_INVALID;
        tokens[pos].value[0] = input[i];
        tokens[pos].value[1] = '\0';
        pos++; i++;
    }

    tokens[pos].type = TOKEN_END;
    tokens[pos].value[0] = '\0';
    return pos;
}
