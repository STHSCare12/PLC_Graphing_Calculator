#include "dualUtils.h"
#include <math.h>

/* Binary operation */
Dual dual_add(Dual a, Dual b) {
    return make_dual(a.real + b.real, a.dual + b.dual);
}

Dual dual_sub(Dual a, Dual b) {
    return make_dual(a.real - b.real, a.dual - b.dual);
}

Dual dual_mul(Dual a, Dual b) {
    return make_dual(a.real * b.real, a.real * b.dual + a.dual * b.real);
}

Dual dual_div(Dual a, Dual b) {
    float inv = 1.0f/b.real;
    return make_dual(a.real * inv, (a.dual * b.real - a.real * b.dual) * inv * inv);
}

Dual dual_pow(Dual a, Dual b) {
    float val = powf(a.real, b.real);
    float deriv = val * (
        b.dual * logf(a.real) +
        b.real * (a.dual / a.real)
    );
    return make_dual(val, deriv);
}

/* Unary operation */
Dual dual_sin(Dual x) {
    return make_dual(sinf(x.real), cosf(x.real) * x.dual);
}

Dual dual_cos(Dual x) {
    return make_dual(cosf(x.real), -sinf(x.real) * x.dual);
}

Dual dual_tan(Dual x) {
    return make_dual(tanf(x.real), x.dual / (cosf(x.real) * cosf(x.real)));
}

Dual dual_exp(Dual x) {
    return make_dual(expf(x.real), expf(x.real) * x.dual);
}

Dual dual_log(Dual x) {
    return make_dual(logf(x.real), x.dual / x.real);
}