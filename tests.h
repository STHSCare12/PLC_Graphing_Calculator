#ifndef TESTS_H
#define TESTS_H

#include "ast.h"
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

#define SIMPLE_TESTS 34
#define DIFF_TESTS 24
#define MAX_TESTS 58

typedef enum {
    TEST_SIMPLE = 1,
    TEST_DIFF,
} EvalTestType;

typedef struct {
    EvalTestType type;
    char eqn[MAX_INPUT];
    double ans;
    char diff[MAX_INPUT];
} Test;

void init_Test(Test* test, char* eqn, double ans, EvalTestType testType, char* diff);
void init_SimpleTests(Test tests[]);
void init_DiffTests(Test tests[]);
int verifyTest(Test test, double results, ASTNode* derivative, int i);
#endif
