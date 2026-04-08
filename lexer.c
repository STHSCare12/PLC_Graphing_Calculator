#include <stdio.h>
#include <string.h>
#include "helper.h"

/*checks if math operator*/
int is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}
/*checks if space*/
int is_space(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}
/*checks if its a digit*/
int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_alpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int is_alnum(char c) {
    return (is_alpha(c) || is_digit(c));
}

int is_valid_number(const char *s) {
    int dot_count = 0;
    int i = 0;
    if (s[i] == '\0') return 0;  /*empty string */
    while (s[i] != '\0') {
        if (s[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;  /*more than one dot -> invalid */
        } else if (!is_digit(s[i])) {
            return 0; 
        }
        i++;
    }
    return 1;
}


int tokenize(const char *input, Token tokens[]) {
    int i = 0;
    int pos = 0;

    while (input[i] != '\0') {
        if (is_space(input[i])) {
            i++; /*skip spaces*/
            continue;
        }

        if (is_digit(input[i]) || input[i] == '.') { /*if its a number or decimal */
            int j = 0;
            while (is_digit(input[i]) || input[i] == '.') {
                if (j < 31) tokens[pos].value[j++] = input[i];
                i++;
            }
            tokens[pos].value[j] = '\0';
            /*validate number first e.g 12.3.2 is invalid */
            if (is_valid_number(tokens[pos].value)) {
                tokens[pos].type = TOKEN_NUMBER;
            } else {
                tokens[pos].type = TOKEN_INVALID;
            }
            pos++;
            continue;
        }

        /*alphabetic letter-> either variable or a function -> identifier*/
        if (is_alpha(input[i])) {
            int j = 0;
            while (is_alnum(input[i])) { /*x1 is counted*/
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
