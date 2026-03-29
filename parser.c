#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "helper.h"
#include "parser.h"

void init_startSymbol(CFGSymbol* symbol, const char* value) {
    symbol->value = value;
    symbol->is_start = 1;
    symbol->is_terminal = 0;
}

void init_terminal(CFGSymbol* symbol, const char* value) {
    symbol->value = value;
    symbol->is_start = 0;
    symbol->is_terminal = 1;
}

void init_nonTerminal(CFGSymbol* symbol, const char* value) {
    symbol->value = value;
    symbol->is_start = 0;
    symbol->is_terminal = 0;
}

CFGProductionRule createProductionRule(CFGSymbol lhs, CFGSymbol rhs[], int rhs_length) {
    CFGProductionRule rule;
    int i;

    if (lhs.is_terminal) {
        fprintf(stderr, "Error: LHS cannot be a terminal\n");
        rule.rhs_length = -1;
        return rule;
    }

    for (i=0;i<rhs_length;i++){
        rule.rhs[i] = rhs[i];
    }

    rule.lhs = lhs;
    rule.rhs_length = rhs_length;

    return rule;
}

void printProductionRule(CFGProductionRule rule) {
    char ruleString[MAX_STRING_LENGTH] = {0};
    int i;

    /* Store before printing to reduce printing overhead*/
    strncat(ruleString, rule.lhs.value, strlen(rule.lhs.value));
    strcat(ruleString, " -->");

    for (i=0;i<rule.rhs_length;i++) {
        strcat(ruleString, " ");
        strncat(ruleString, rule.rhs[i].value, strlen(rule.rhs[i].value));
    }

    i = printf("%s\n", ruleString);
    if (i < 0) fprintf(stderr, "Error: Production Rule failed to print\n");
}

void init_CFG(CFG* cfg, 
              CFGSymbol startSymbol, 
              CFGSymbol symbols[], 
              int symbol_count, 
              CFGProductionRule rules[], 
              int rule_count) {
    int i;

    for (i=0;i<symbol_count;i++) {
        cfg->symbols[i] = symbols[i];
    }
    
    for (i=0;i<rule_count;i++) {
        cfg->rules[i] = rules[i];
    }

    cfg->startSymbol = startSymbol;
    cfg->symbol_count = symbol_count;
    cfg->rule_count = rule_count;
}

void printCFG(const CFG cfg) {
    int i;

    printf("Start Symbol: %s\n", cfg.startSymbol.value);
    printf("Production Rules:\n");
    for (i=0;i<MAX_RULES;i++) {
        printProductionRule(cfg.rules[i]);
    }
}

/* Currently Unused; Not sure if would be useful; Used to double check syntax for terminal num */
int check_isNum(char* string) {
    char* pattern = "^[+-]?(0|[1-9][0-9]*)(\\.[0-9]+|e-?[0-9]+)?$";
    regex_t regex;
    int status = regcomp(&regex, pattern, REG_EXTENDED);

    if (status != 0) {
        fprintf(stderr, "Error: Failed to compile regex pattern\n");
        return 1;
    }

    status = regexec(&regex, string, 0, NULL, 0);
    if (status == 0) {
        printf("Success: %s is a valid number\n", string);
    }
    else if (status == REG_NOMATCH) {
        printf("Failure: %s is not a valid number\n", string);
    }
    else {
        fprintf(stderr, "Error: Failed to match pattern to RegEx\n");
        return 1;
    }

    regfree(&regex);
    return 0;
}

int parse(Token tokens[], int choice) {
    CFGSymbol S, DIFF, E, T, P, F, NE, NT, NP, NF, FUNC; /* Non Terminals */
    CFGSymbol Y, VAR, NUM, EQ, ADD, MINUS, MULTIPLY, DIV, POWER, LPAREN, RPAREN; /* Terminals */
    CFGSymbol AOPS, MOPS; /* Additive Operators (+,-), Multiplicative Ops (*,/) */
    CFGSymbol SIN, COS, TAN, EXP, LOG; /* Function operations*/

    CFGSymbol rhs1[1], rhs2[3], rhs3[3], rhs4[1], rhs5[3], rhs6[3], rhs7[1], rhs8[1], rhs9[3], rhs10[3], rhs11[4], rhs12[3], rhs13[1], rhs14[1], rhs15[2]; 
    CFGSymbol rhs16[1], rhs17[3], rhs18[1], rhs19[3], rhs20[3], rhs21[1], rhs22[1], rhs23[3], rhs24[4], rhs25[3], rhs26[1], rhs27[2];
    CFGSymbol rhs28[1], rhs29[1], rhs30[1], rhs31[1];
    CFGSymbol rhs32[1], rhs33[1], rhs34[1], rhs35[1], rhs36[1];

    CFGProductionRule rule1, rule2, rule3, rule4, rule5, rule6, rule7, rule8, rule9, rule10, rule11, rule12, rule13, rule14, rule15;
    CFGProductionRule rule16, rule17, rule18, rule19, rule20, rule21, rule22, rule23, rule24, rule25, rule26, rule27;
    CFGProductionRule rule28, rule29, rule30, rule31;
    CFGProductionRule rule32, rule33, rule34, rule35, rule36;

    CFGProductionRule rules[MAX_RULES];
    CFGSymbol symbols[MAX_SYMBOLS];
    CFG cfg;

    int symbol_count, rule_count;

    /* NON TERMINALS */
    /* Differentiation + Graphing */
    init_startSymbol(&S, "Start");
    init_nonTerminal(&DIFF, "DiffStatement");
    init_nonTerminal(&E, "Expr");
    init_nonTerminal(&T, "Term");
    init_nonTerminal(&P, "Power");
    init_nonTerminal(&F, "Factor");

    /* Normal/Numeric Evaluation */
    init_nonTerminal(&NE, "NumExpr");
    init_nonTerminal(&NT, "NumTerm");
    init_nonTerminal(&NP, "NumPower");
    init_nonTerminal(&NF, "NumFactor");

    /* Operators */
    init_nonTerminal(&AOPS, "AdditiveOp");
    init_nonTerminal(&MOPS, "MultiplicativeOp");
    init_nonTerminal(&FUNC, "Func");
    /* NON TERMINALS */

    /* TERMINALS */
    init_terminal(&Y, "y");
    init_terminal(&VAR, "x");
    init_terminal(&NUM, "num");
    init_terminal(&EQ, "=");
    init_terminal(&ADD, "+");
    init_terminal(&MINUS, "-");
    init_terminal(&MULTIPLY, "*");
    init_terminal(&DIV, "/");
    init_terminal(&POWER, "^");
    init_terminal(&LPAREN, "(");
    init_terminal(&RPAREN, ")");
    
    /* functions */
    init_terminal(&SIN, "sin");
    init_terminal(&COS, "cos");
    init_terminal(&TAN, "tan");
    init_terminal(&EXP, "exp");
    init_terminal(&LOG, "log");
    /* TERMINALS */

    /* PRODUCTION RULES */
    /* Differentiation + Graphing */
    /* Start --> DiffStatement */
    rhs1[0] = DIFF;
    rule1 = createProductionRule(S, rhs1, 1);

    /* DiffStatement --> y = Expr */
    rhs2[0] = Y; rhs2[1] = EQ; rhs2[2] = E;
    rule2 = createProductionRule(DIFF, rhs2, 3);

    /* Expr --> Expr AdditiveOp Term*/
    rhs3[0] = E; rhs3[1] = AOPS; rhs3[2] = T;
    rule3 = createProductionRule(E, rhs3, 3);

    /* Expr --> Term */
    rhs4[0] = T;
    rule4 = createProductionRule(E, rhs4, 1);

    /* Term --> Term MultiplicativeOp Power */
    rhs5[0] = T; rhs5[1] = MOPS; rhs5[2] = P;
    rule5 = createProductionRule(T, rhs5, 3);

    /* Term --> Term MultiplicativeOp Factor */
    rhs6[0] = T; rhs6[1] = MOPS; rhs6[2] = F;
    rule6 = createProductionRule(T, rhs6, 3);

    /* Term --> Power */
    rhs7[0] = P;
    rule7 = createProductionRule(T, rhs7, 1);
    
    /* Term --> Factor */
    rhs8[0] = F;
    rule8 = createProductionRule(T, rhs8, 1);

    /* Power --> Factor ^ num */
    rhs9[0] = F; rhs9[1] = POWER; rhs9[2] = NUM;
    rule9 = createProductionRule(P, rhs9, 3);
    
    /* Power --> Factor ^ var */
    rhs10[0] = F; rhs10[1] = POWER; rhs10[2] = VAR;
    rule10 = createProductionRule(P, rhs10, 3);

    /* Factor --> func ( Expr ) */
    rhs11[0] = FUNC; rhs11[1] = LPAREN; rhs11[2] = E; rhs11[3] = RPAREN;
    rule11 = createProductionRule(F, rhs11, 4);

    /* Factor --> ( Expr ) */
    rhs12[0] = LPAREN; rhs12[1] = E; rhs12[2] = RPAREN;
    rule12 = createProductionRule(F, rhs12, 3);

    /* Factor --> num */
    rhs13[0] = NUM;
    rule13 = createProductionRule(F, rhs13, 1);

    /* Factor --> var */
    rhs14[0] = VAR;
    rule14 = createProductionRule(F, rhs14, 1);

    /* Factor --> - Factor */
    rhs15[0] = MINUS; rhs15[1] = F;
    rule15 = createProductionRule(F, rhs15, 2);
    /* Differentiation + Graphing */

    /* Normal/Numeric Evaluation */
    /* Start --> NumExpr*/
    rhs16[0] = NE;
    rule16 = createProductionRule(S, rhs16, 1);

    /* NumExpr --> NumExpr AdditiveOp NumTerm*/
    rhs17[0] = NE; rhs17[1] = AOPS; rhs17[2] = NT;
    rule17 = createProductionRule(NE, rhs17, 3);

    /* NumExpr --> NumTerm */
    rhs18[0] = NT;
    rule18 = createProductionRule(NE, rhs18, 1);

    /* NumTerm --> NumTerm MultiplicativeOp NumPower */
    rhs19[0] = NT; rhs19[1] = MOPS; rhs19[2] = NP;
    rule19 = createProductionRule(NT, rhs19, 3);

    /* NumTerm --> NumTerm MultiplicativeOp NumFactor */
    rhs20[0] = NT; rhs20[1] = MOPS; rhs20[2] = NF;
    rule20 = createProductionRule(NT, rhs20, 3);

    /* NumTerm --> NumPower */
    rhs21[0] = NP;
    rule21 = createProductionRule(NT, rhs21, 1);

    /* NumTerm --> NumFactor */
    rhs22[0] = NF;
    rule22 = createProductionRule(NT, rhs22, 1);

    /* NumPower --> NumFactor ^ num */
    rhs23[0] = NF; rhs23[1] = POWER; rhs23[2] = NUM;
    rule23 = createProductionRule(NP, rhs23, 3);
    
    /* NumFactor --> func ( NumExpr ) */
    rhs24[0] = FUNC; rhs24[1] = LPAREN; rhs24[2] = NE; rhs24[3] = RPAREN;
    rule24 = createProductionRule(NF, rhs24, 4);

    /* NumFactor --> ( NumExpr ) */
    rhs25[0] = LPAREN; rhs25[1] = NE; rhs25[2] = RPAREN;
    rule25 = createProductionRule(NF, rhs25, 3);

    /* NumFactor --> num */
    rhs26[0] = NUM;
    rule26 = createProductionRule(NF, rhs26, 1);

    /* NumFactor --> - NumFactor */
    rhs27[0] = MINUS; rhs27[1] = NF;
    rule27 = createProductionRule(NF, rhs27, 2);
    /* Normal/Numeric Evaluation */

    /* Operators */
    /* AdditiveOp --> + */
    rhs28[0] = ADD;
    rule28 = createProductionRule(AOPS, rhs28, 1);

    /* AdditiveOp --> - */
    rhs29[0] = MINUS;
    rule29 = createProductionRule(AOPS, rhs29, 1);

    /* MultiplicativeOp --> * */
    rhs30[0] = MULTIPLY;
    rule30 = createProductionRule(MOPS, rhs30, 1);

    /* MultiplicativeOp --> / */
    rhs31[0] = DIV;
    rule31 = createProductionRule(MOPS, rhs31, 1);
    /* Operators */

    /* Functions */
    rhs32[0] = SIN;
    rule32 = createProductionRule(FUNC, rhs32, 1);

    rhs33[0] = COS;
    rule33 = createProductionRule(FUNC, rhs33, 1);
    
    rhs34[0] = TAN;
    rule34 = createProductionRule(FUNC, rhs34, 1);
    
    rhs35[0] = EXP;
    rule35 = createProductionRule(FUNC, rhs35, 1);
    
    rhs36[0] = LOG;
    rule36 = createProductionRule(FUNC, rhs36, 1);
    /* Functions */
    /* PRODUCTION RULES */

    /* INITIALIZE SYMBOLS ARRAY */
    symbol_count = MAX_SYMBOLS;
    symbols[0] = S; 
    symbols[1] = DIFF;
    symbols[2] = E; 
    symbols[3] = T; 
    symbols[4] = P; 
    symbols[5] = F;
    symbols[6] = NE; 
    symbols[7] = NT; 
    symbols[8] = NP; 
    symbols[9] = NF;
    symbols[10] = FUNC; 
    symbols[11] = Y; 
    symbols[12] = VAR; 
    symbols[13] = NUM; 
    symbols[14] = EQ; 
    symbols[15] = ADD; 
    symbols[16] = MINUS; 
    symbols[17] = MULTIPLY; 
    symbols[18] = DIV; 
    symbols[19] = POWER; 
    symbols[20] = LPAREN; 
    symbols[21] = RPAREN;
    symbols[22] = AOPS; 
    symbols[23] = MOPS;
    symbols[24] = SIN;
    symbols[25] = COS;
    symbols[26] = TAN;
    symbols[27] = EXP;
    symbols[28] = LOG;

    /* INITIALIZE RULES ARRAY */
    rule_count = MAX_RULES;
    rules[0]  = rule1; 
    rules[1]  = rule2; 
    rules[2]  = rule3; 
    rules[3]  = rule4; 
    rules[4]  = rule5; 
    rules[5]  = rule6; 
    rules[6]  = rule7; 
    rules[7]  = rule8; 
    rules[8]  = rule9; 
    rules[9]  = rule10; 
    rules[10] = rule11; 
    rules[11] = rule12; 
    rules[12] = rule13; 
    rules[13] = rule14; 
    rules[14] = rule15;
    rules[15] = rule16; 
    rules[16] = rule17; 
    rules[17] = rule18; 
    rules[18] = rule19; 
    rules[19] = rule20; 
    rules[20] = rule21; 
    rules[21] = rule22; 
    rules[22] = rule23; 
    rules[23] = rule24; 
    rules[24] = rule25; 
    rules[25] = rule26; 
    rules[26] = rule27;
    rules[27] = rule28; 
    rules[28] = rule29; 
    rules[29] = rule30; 
    rules[30] = rule31;
    rules[31] = rule32;
    rules[32] = rule33;
    rules[33] = rule34;
    rules[34] = rule35;
    rules[35] = rule36;

    /* INITALIZE CFG */
    init_CFG(&cfg, S, symbols, symbol_count, rules, rule_count);
    printCFG(cfg);

    /* TO-DO: Figure out method of syntax analysis using CFG to be used for evaluation*/

    return 1;
}