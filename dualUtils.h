#ifndef DUAL_UTILS_H
#define DUAL_UTILS_H
#pragma once
#include <math.h>

/* Dual number struct */
typedef struct Dual {
    float real; /* actual value f(x) */
    float dual; /* derivative f'(x) */
} Dual;

/* Unary operation */
typedef struct DualUnaryOp {
    Dual (*apply)(Dual);
} DualUnaryOp;

/* Binary operation */
typedef struct DualBinaryOp {
    Dual (*apply)(Dual, Dual);
} DualBinaryOp;

/* Create a dual number */
static inline Dual make_dual(float real_part, float dual_part) {
    return (Dual){real_part, dual_part};
}

// #define DEFINE_DUAL_UNARY_OP(NAME, FN, FN_PRIME)                           \
//     Dual NAME##_apply(Dual x) {                                             \
//         return (Dual){                                                     \
//             .real = (FN)(x.real),                                          \
//             .dual = (FN_PRIME)(x.real) * x.dual                            \
//         };                                                                 \
//     }                                                                      \
//     const DualUnaryOp NAME = { NAME##_apply };

// #define DEFINE_DUAL_BINARY_OP(NAME, BODY)                                  \
//     Dual NAME##_apply(Dual a, Dual b) BODY                   \
//     const DualBinaryOp NAME = { NAME##_apply };

extern const DualUnaryOp DUAL_SIN;
extern const DualUnaryOp DUAL_COS;
extern const DualUnaryOp DUAL_TAN;
extern const DualUnaryOp DUAL_EXP;
extern const DualUnaryOp DUAL_LOG;

extern const DualBinaryOp DUAL_ADD;
extern const DualBinaryOp DUAL_SUB;
extern const DualBinaryOp DUAL_MUL;
extern const DualBinaryOp DUAL_DIV;
extern const DualBinaryOp DUAL_POW;
#endif