#ifndef HELPER_H
#define HELPER_H

#define MAX_INPUT 256
#define MAX_TOKENS 128

void trim(char *str);

typedef enum {
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_IDENTIFIER,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_END,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    char value[32];
} Token;

int read_input(int argc, char *argv[], char *buffer, int mode, Token tokens[]);
void process_input(char *input, int mode, Token tokens[]);
void print_tokens(Token tokens[]);
int tokenize(const char *input, Token tokens[]);
#endif
