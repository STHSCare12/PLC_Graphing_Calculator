#include "tests.h"
#include <stdio.h>

void init_Test(Test* test, char* eqn, double ans, EvalTestType testType, char* diff) {
    int i;
    test->type = testType;
    test->ans = ans;

    for(i=0;i<MAX_INPUT;i++) {
        test->eqn[i] = eqn[i];
    }

    if (diff) {
        for(i=0;i<MAX_INPUT;i++) {
            test->diff[i] = diff[i];
        }
    }
}

void init_SimpleTests(Test tests[]) {
    Test ADD_1, ADD_2, SUB_1, SUB_2, SUB_3, SUB_4;
    Test MUL_1, MUL_2, MUL_3, MUL_4, MUL_5, MUL_6, DIV_1, DIV_2, DIV_3, DIV_4, DIV_5, DIV_6, POW_1, POW_2, POW_3, POW_4, POW_5, POW_6, POW_7;
    Test ADDITIVE_1, ADDITIVE_2, ADDITIVE_3, ADDITIVE_4;
    Test MULTIPLICATIVE_1;
    Test ALL_1, ALL_2;
    Test UNARYMINUS_1, UNARYMINUS_2;

    init_Test(&ADD_1, "+1+2", 3, TEST_SIMPLE, NULL);
    init_Test(&ADD_2, "INT_MAX+1", (double)INT_MAX+1, TEST_SIMPLE, NULL);
    init_Test(&SUB_1, "-1-2", -3, TEST_SIMPLE, NULL);
    init_Test(&SUB_2, "INT_MIN-1", (double)INT_MIN-1, TEST_SIMPLE, NULL);
    init_Test(&SUB_3, "1-(-1)", 2, TEST_SIMPLE, NULL);
    init_Test(&SUB_4, "1-(-(-(-(-1))))", 0,TEST_SIMPLE, NULL);
    
    init_Test(&MUL_1, "*1", 0, TEST_SIMPLE, NULL);
    init_Test(&MUL_2, "INT_MAX*0", 0, TEST_SIMPLE, NULL);
    init_Test(&MUL_3, "INT_MAX*-1", -((double)INT_MAX), TEST_SIMPLE, NULL);
    init_Test(&MUL_4, "INT_MIN*-1", -((double)INT_MIN), TEST_SIMPLE, NULL);
    init_Test(&MUL_5, "INT_MIN*0", 0, TEST_SIMPLE, NULL);
    init_Test(&MUL_6, "(1*0)*(1*-1)", 0, TEST_SIMPLE, NULL);
    init_Test(&DIV_1, "2/1", 2, TEST_SIMPLE, NULL);
    init_Test(&DIV_2, "/0", 0.0/0.0, TEST_SIMPLE, NULL);
    init_Test(&DIV_3, "/1", 0, TEST_SIMPLE, NULL);
    init_Test(&DIV_4, "2/2", 1, TEST_SIMPLE, NULL);
    init_Test(&DIV_5, "2/-1", -2, TEST_SIMPLE, NULL);
    init_Test(&DIV_6, "1/(1/0)", 1.0/(1.0/0.0), TEST_SIMPLE, NULL);
    init_Test(&POW_1, "^2", 0, TEST_SIMPLE, NULL);
    init_Test(&POW_2, "INT_MAX^INT_MAX", INFINITY, TEST_SIMPLE, NULL);
    init_Test(&POW_3, "INT_MAX^0", 1, TEST_SIMPLE, NULL);
    init_Test(&POW_4, "-1^2", -1, TEST_SIMPLE, NULL);
    init_Test(&POW_5, "(-1)^2", 1, TEST_SIMPLE, NULL);
    init_Test(&POW_6, "1^-2", 1, TEST_SIMPLE, NULL);
    init_Test(&POW_7, "-1^-2", -1, TEST_SIMPLE, NULL);

    init_Test(&ADDITIVE_1, "INT_MAX+INT_MIN", -1, TEST_SIMPLE, NULL);
    init_Test(&ADDITIVE_2, "-1+-1", -2, TEST_SIMPLE, NULL);
    init_Test(&ADDITIVE_3, "(1+2)-(1+2)", 0, TEST_SIMPLE, NULL);
    init_Test(&ADDITIVE_4, "(1+2)-(-(1+2))", 6, TEST_SIMPLE, NULL);
    
    init_Test(&MULTIPLICATIVE_1, "2*4/2", 4, TEST_SIMPLE, NULL);
    
    init_Test(&ALL_1, "2*2+2/2-2", 3, TEST_SIMPLE, NULL);
    init_Test(&ALL_2, "2*(2+2-2)/2", 2, TEST_SIMPLE, NULL);

    init_Test(&UNARYMINUS_1, "-2^4", -16, TEST_SIMPLE, NULL);
    init_Test(&UNARYMINUS_2, "-2^-4", -0.0625, TEST_SIMPLE, NULL);

    tests[0]  = ADD_1; 
    tests[1]  = ADD_2; 
    tests[2]  = SUB_1; 
    tests[3]  = SUB_2; 
    tests[4]  = SUB_3; 
    tests[5]  = SUB_4; 
    tests[6]  = MUL_1; 
    tests[7]  = MUL_2; 
    tests[8]  = MUL_3; 
    tests[9]  = MUL_4; 
    tests[10] = MUL_5; 
    tests[11] = MUL_6; 
    tests[12] = DIV_1; 
    tests[13] = DIV_2; 
    tests[14] = DIV_3;
    tests[15] = DIV_4; 
    tests[16] = DIV_5; 
    tests[17] = DIV_6; 
    tests[18] = POW_1; 
    tests[19] = POW_2; 
    tests[20] = POW_3; 
    tests[21] = POW_4; 
    tests[22] = POW_5; 
    tests[23] = POW_6; 
    tests[24] = POW_7; 
    tests[25] = ADDITIVE_1; 
    tests[26] = ADDITIVE_2;
    tests[27] = ADDITIVE_3; 
    tests[28] = ADDITIVE_4; 
    tests[29] = MULTIPLICATIVE_1; 
    tests[30] = ALL_1;
    tests[31] = ALL_2;
    tests[32] = UNARYMINUS_1;
    tests[33] = UNARYMINUS_2;
}

void init_DiffTests(Test tests[]) {
    Test ADD_1, SUB_1, SUB_2; 
    Test MUL_1, MUL_2, MUL_3, MUL_4, DIV_1, DIV_2, DIV_3, DIV_4, POW_1, POW_2, POW_3;
    Test TRIGO_1, TRIGO_2, TRIGO_3, TRIGO_4, TRIGO_5, TRIGO_6, TRIGO_7, TRIGO_8, TRIGO_9, TRIGO_0;

    init_Test(&ADD_1, "1*x+2*x", 3, TEST_DIFF, "3");
    init_Test(&SUB_1, "-1*x-1*x", -2, TEST_DIFF, "-2");
    init_Test(&SUB_2, "1*x-1*x", 0, TEST_DIFF, "0");
    
    init_Test(&MUL_1, "1*x*1*x", 4, TEST_DIFF, "(x + x)");
    init_Test(&MUL_2, "(1*x)*(1*x)", 4, TEST_DIFF, "(x + x)");
    init_Test(&MUL_3, "-1*x*-1*x", 4, TEST_DIFF, "(x + x)");
    init_Test(&MUL_4, "-1*x*1*x", -4, TEST_DIFF, "((-1.000000 * x) + (-1.000000 * x))");
    init_Test(&DIV_1, "2*x/2*x", 4, TEST_DIFF, "(x + ((2.000000 * x) / 2.000000))");
    init_Test(&DIV_2, "-x/-x", 0, TEST_DIFF, "0.000000");
    init_Test(&DIV_3, "x/2", 0.5, TEST_DIFF, "0.500000");
    init_Test(&DIV_4, "x/-x", 0, TEST_DIFF, "0.000000");
    init_Test(&POW_1, "x^2", 4, TEST_DIFF, "(2.000000 * x)");
    init_Test(&POW_2, "-x^2", -4, TEST_DIFF, "-(2.000000 * x)");
    init_Test(&POW_3, "x^x", pow(2,2)*(log10(2)+1), TEST_DIFF, "((x ^ x) * (log(x) + (x * (1.000000 / x)))))");

    init_Test(&TRIGO_1, "sin(x)", cos(2*(M_PI/180)), TEST_DIFF, "cos(x)");
    init_Test(&TRIGO_2, "sin(-x)", -cos(2*(M_PI/180)), TEST_DIFF, "(cos(-x) * -1.000000)");
    init_Test(&TRIGO_3, "cos(x)", -sin(2*(M_PI/180)), TEST_DIFF, "-sin(x)");
    init_Test(&TRIGO_4, "cos(-x)", sin(-2*(M_PI/180)), TEST_DIFF, "(-sin(-x) * -1.000000)");
    init_Test(&TRIGO_5, "tan(x)", 1/(pow(cos(2*(M_PI/180)), 2)), TEST_DIFF, "(1.000000 / (cos(x) ^ 2.000000))");
    init_Test(&TRIGO_6, "tan(-x)", -(1/(pow(cos(2*(M_PI/180)), 2))), TEST_DIFF, "((1.000000 / (cos(-x) ^ 2.000000)) * -1.000000)");
    init_Test(&TRIGO_7, "exp(x)", exp(2), TEST_DIFF, "exp(x)");
    init_Test(&TRIGO_8, "exp(-x)", -exp(-2), TEST_DIFF, "(exp(-x) * -1.000000)");
    init_Test(&TRIGO_9, "log(x)", 1/(2*log(10)), TEST_DIFF, "(1.000000 / (x * 2.302585))");
    init_Test(&TRIGO_0, "log(-x)", 1/(2*log(10)), TEST_DIFF, "((1.000000 / (-x * 2.302585)) * -1.000000)");

    tests[0]  = ADD_1; 
    tests[1]  = SUB_1; 
    tests[2]  = SUB_2; 
    tests[3]  = MUL_1; 
    tests[4]  = MUL_2; 
    tests[5]  = MUL_3; 
    tests[6]  = MUL_4; 
    tests[7]  = DIV_1; 
    tests[8]  = DIV_2; 
    tests[9]  = DIV_3;
    tests[10] = DIV_4; 
    tests[11] = POW_1; 
    tests[12] = POW_2; 
    tests[13] = POW_3; 
    tests[14] = TRIGO_1;
    tests[15] = TRIGO_2; 
    tests[16] = TRIGO_3; 
    tests[17] = TRIGO_4; 
    tests[18] = TRIGO_5; 
    tests[19] = TRIGO_6; 
    tests[20] = TRIGO_7; 
    tests[21] = TRIGO_8; 
    tests[22] = TRIGO_9; 
    tests[23] = TRIGO_0; 
}

int verifyTest(Test test, double result, ASTNode* derivative, int i) {
    if (test.type == TEST_SIMPLE) {
        if (result == test.ans || (isnan(result) && isnan(test.ans))) {
            printf("Test %d: Success (%s == %f)\n", i, test.eqn, test.ans);
            return 0;
        }
        else {
            printf("Test %d: Failed (%s = %f != %f)\n", i, test.eqn, result, test.ans);
            return 1;
        }
    }
    else if (test.type == TEST_DIFF) {
        if (result == test.ans || (fabsf(result - test.ans) < 0.0000001f)) {
            printf("Test %d: Success (%s == %f)\n", i, test.diff, test.ans);
            return 0;
        }
        else {
            printf("Test %d: Failed (%s = %f != %f)\n", i, test.diff, result, test.ans);
            return 1;
        }
    }
    return 0;
}
