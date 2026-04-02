#include "dualUtils.h"
#include <math.h>

// DEFINE_DUAL_BINARY_OP(
//     DUAL_ADD, {
//         return make_dual(a.real + b.real, a.dual + b.dual);
//     }
// )

// DEFINE_DUAL_BINARY_OP(
//     DUAL_SUB, {
//         return make_dual(a.real - b.real, a.dual - b.dual);
//     }
// )

// DEFINE_DUAL_BINARY_OP(
//     DUAL_MUL, {
//         return make_dual(a.real * b.real, a.real * b.dual + a.dual * b.real);
//     }
// )

// DEFINE_DUAL_BINARY_OP(
//     DUAL_DIV, {
//         float inv = 1.0f/b.real;
//         return make_dual(a.real * inv, (a.dual * b.real - a.real * b.dual) * inv * inv);
//     }
// )

// DEFINE_DUAL_BINARY_OP(
//     DUAL_POW, {
//         float val = powf(a.real, b.real);

//         float deriv = val * (
//             b.dual * logf(a.real) +
//             b.real * (a.dual / a.real)
//         );

//         return make_dual(val, deriv);
//     }
// )

// DEFINE_DUAL_UNARY_OP(DUAL_SIN, sinf, cosf)

// static inline float neg_sinf(float x) { return -sinf(x); }

// DEFINE_DUAL_UNARY_OP(DUAL_COS, cosf, neg_sinf)

// static inline float tan_prime(float x) {
//     float c = cosf(x);
//     return 1.0f / (c * c);
// }

// DEFINE_DUAL_UNARY_OP(DUAL_TAN, tanf, tan_prime)

// DEFINE_DUAL_UNARY_OP(DUAL_EXP, expf, expf)

// static inline float log_prime(float x) {
//     return 1.0f / x;
// }

// DEFINE_DUAL_UNARY_OP(DUAL_LOG, logf, log_prime)


/* Unary operation functions */
Dual DUAL_SIN_apply(Dual x) { return (Dual){sinf(x.real), cosf(x.real) * x.dual}; }
Dual DUAL_COS_apply(Dual x) { return (Dual){cosf(x.real), -sinf(x.real) * x.dual}; }
Dual DUAL_TAN_apply(Dual x) { return (Dual){tanf(x.real), x.dual / (cosf(x.real) * cosf(x.real))}; }
Dual DUAL_EXP_apply(Dual x) { return (Dual){expf(x.real), expf(x.real) * x.dual}; }
Dual DUAL_LOG_apply(Dual x) { return (Dual){logf(x.real), x.dual / x.real}; }

/* Unary operation structs */
const DualUnaryOp DUAL_SIN = { DUAL_SIN_apply };
const DualUnaryOp DUAL_COS = { DUAL_COS_apply };
const DualUnaryOp DUAL_TAN = { DUAL_TAN_apply };
const DualUnaryOp DUAL_EXP = { DUAL_EXP_apply };
const DualUnaryOp DUAL_LOG = { DUAL_LOG_apply };

/* Binary operation functions */
Dual DUAL_ADD_apply(Dual a, Dual b) { return make_dual(a.real + b.real, a.dual + b.dual); }
Dual DUAL_SUB_apply(Dual a, Dual b) { return make_dual(a.real - b.real, a.dual - b.dual); }
Dual DUAL_MUL_apply(Dual a, Dual b) { return make_dual(a.real*b.real, a.real*b.dual + a.dual*b.real); }
Dual DUAL_DIV_apply(Dual a, Dual b) { float inv = 1.0f/b.real; return make_dual(a.real*inv, (a.dual*b.real - a.real*b.dual)*inv*inv); }
Dual DUAL_POW_apply(Dual a, Dual b) { 
    float val = powf(a.real, b.real);
    float deriv = val * (b.dual * logf(a.real) + b.real * (a.dual / a.real));
    return make_dual(val, deriv);
}

/* Binary operation structs */
const DualBinaryOp DUAL_ADD = { DUAL_ADD_apply };
const DualBinaryOp DUAL_SUB = { DUAL_SUB_apply };
const DualBinaryOp DUAL_MUL = { DUAL_MUL_apply };
const DualBinaryOp DUAL_DIV = { DUAL_DIV_apply };
const DualBinaryOp DUAL_POW = { DUAL_POW_apply };