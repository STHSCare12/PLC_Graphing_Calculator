#ifndef DUAL_UTILS_H
#define DUAL_UTILS_H
#pragma once
#include <math.h>

/* Dual number struct */
typedef struct Dual {
    float real; /* actual value f(x) */
    float dual; /* derivative f'(x) */
} Dual;

/* Create a dual number */
Dual make_dual(float real_part, float dual_part);

/* Unary operation */
Dual dual_sin(Dual x);
Dual dual_cos(Dual x);
Dual dual_tan(Dual x);
Dual dual_exp(Dual x);
Dual dual_log(Dual x);

/* Binary  operation */
Dual dual_add(Dual a, Dual b);
Dual dual_sub(Dual a, Dual b);
Dual dual_mul(Dual a, Dual b);
Dual dual_div(Dual a, Dual b);
Dual dual_pow(Dual a, Dual b);

#endif