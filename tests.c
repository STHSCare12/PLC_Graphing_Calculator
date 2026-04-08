#include "tests.h"
#include <stdio.h>

void init_Test(Test* test, char* eqn, double ans, EvalTestType testType) {
    int i;
    test->type = testType;
    test->ans = ans;

    for(i=0;i<MAX_INPUT;i++) {
        test->eqn[i] = eqn[i];
    }
}

void init_SimpleTests(Test tests[]) {
    Test ADD_1, ADD_2, SUB_1, SUB_2, SUB_3, SUB_4;
    Test MUL_1, MUL_2, MUL_3, MUL_4, MUL_5, MUL_6, DIV_1, DIV_2, DIV_3, DIV_4, DIV_5, DIV_6, POW_1, POW_2, POW_3, POW_4, POW_5, POW_6, POW_7;
    Test ADDITIVE_1, ADDITIVE_2, ADDITIVE_3, ADDITIVE_4;
    Test MULTIPLICATIVE_1;
    Test ALL_1, ALL_2;

    init_Test(&ADD_1, "+1+2", 3, TEST_SIMPLE);
    init_Test(&ADD_2, "INT_MAX+1", (double)INT_MAX+1, TEST_SIMPLE);
    init_Test(&SUB_1, "-1-2", -3, TEST_SIMPLE);
    init_Test(&SUB_2, "INT_MIN-1", (double)INT_MIN-1, TEST_SIMPLE);
    init_Test(&SUB_3, "1-(-1)", 2, TEST_SIMPLE);
    init_Test(&SUB_4, "1-(-(-(-(-1))))", 0,TEST_SIMPLE);
    
    init_Test(&MUL_1, "*1", 0, TEST_SIMPLE);
    init_Test(&MUL_2, "INT_MAX*0", 0, TEST_SIMPLE);
    init_Test(&MUL_3, "INT_MAX*-1", -((double)INT_MAX), TEST_SIMPLE);
    init_Test(&MUL_4, "INT_MIN*-1", -((double)INT_MIN), TEST_SIMPLE);
    init_Test(&MUL_5, "INT_MIN*0", 0, TEST_SIMPLE);
    init_Test(&MUL_6, "(1*0)*(1*-1)", 0, TEST_SIMPLE);
    init_Test(&DIV_1, "2/1", 2, TEST_SIMPLE);
    init_Test(&DIV_2, "/0", 0.0/0.0, TEST_SIMPLE);
    init_Test(&DIV_3, "/1", 0, TEST_SIMPLE);
    init_Test(&DIV_4, "2/2", 1, TEST_SIMPLE);
    init_Test(&DIV_5, "2/-1", -2, TEST_SIMPLE);
    init_Test(&DIV_6, "1/(1/0)", 1.0/(1.0/0.0), TEST_SIMPLE);
    init_Test(&POW_1, "^2", 0, TEST_SIMPLE);
    init_Test(&POW_2, "INT_MAX^INT_MAX", INFINITY, TEST_SIMPLE);
    init_Test(&POW_3, "INT_MAX^0", 1, TEST_SIMPLE);
    init_Test(&POW_4, "-1^2", 1, TEST_SIMPLE);
    init_Test(&POW_5, "-1^3", -1, TEST_SIMPLE);
    init_Test(&POW_6, "1^-2", 1, TEST_SIMPLE);
    init_Test(&POW_7, "-1^-2", 1, TEST_SIMPLE);

    init_Test(&ADDITIVE_1, "INT_MAX+INT_MIN", -1, TEST_SIMPLE);
    init_Test(&ADDITIVE_2, "-1+-1", -2, TEST_SIMPLE);
    init_Test(&ADDITIVE_3, "(1+2)-(1+2)", 0, TEST_SIMPLE);
    init_Test(&ADDITIVE_4, "(1+2)-(-(1+2))", 6, TEST_SIMPLE);
    
    init_Test(&MULTIPLICATIVE_1, "2*4/2", 4, TEST_SIMPLE);
    
    init_Test(&ALL_1, "2*2+2/2-2", 3, TEST_SIMPLE);
    init_Test(&ALL_2, "2*(2+2-2)/2", 2, TEST_SIMPLE);

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
}

int verifyTest(Test test, double result, int i) {
    if (result == test.ans || (isnan(result) && isnan(test.ans))) {
        printf("Test %d: Success (%s == %f)\n", i, test.eqn, test.ans);
        return 0;
    }
    else {
        printf("Test %d: Failed (%s = %f != %f)\n", i, test.eqn, result, test.ans);
        return 1;
    }

}