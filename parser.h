#ifndef PARSER_H
#define PARSER_H

#define MAX_STRING_LENGTH 1024
#define MAX_SYMBOLS 29
#define MAX_RULES 36
#define MAX_RHS 5

typedef struct {
    const char* value;
    int is_terminal;
    int is_start;
} CFGSymbol;

typedef struct {
    CFGSymbol lhs;
    CFGSymbol rhs[MAX_RHS];
    int rhs_length;
} CFGProductionRule;

typedef struct {
    CFGSymbol startSymbol;
    CFGSymbol symbols[MAX_SYMBOLS];
    int symbol_count;
    CFGProductionRule rules[MAX_RULES];
    int rule_count;
} CFG;

void init_startSymbol(CFGSymbol* symbol, const char* value);
void init_terminal(CFGSymbol* symbol, const char* value);
void init_nonTerminal(CFGSymbol* symbol, const char* value);
CFGProductionRule createProductionRule(CFGSymbol lhs, CFGSymbol rhs[], int rhs_length);
void printProductionRule(CFGProductionRule rule);
void init_CFG(CFG* cfg, CFGSymbol startSymbol, CFGSymbol symbols[], int symbol_count, CFGProductionRule rules[], int rule_count);
void printCFG(const CFG cfg);
int parse(Token tokens[], int choice);
#endif