#ifndef TESTS_H
#define TESTS_H

#include "ast.h"
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

#define MAX_TESTS 32

typedef enum {
    TEST_SIMPLE = 1,
    TEST_DIFF,
    TEST_GRAPH,
} EvalTestType;

typedef struct {
    EvalTestType type;
    char eqn[MAX_INPUT];
    double ans;
} Test;

void init_Test(Test* test, char* eqn, double ans, EvalTestType testType);
void init_SimpleTests(Test tests[]);
int verifyTest(Test test, double results, int i);
#endif