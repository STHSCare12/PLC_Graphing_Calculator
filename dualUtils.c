#include "dualUtils.h"
#include <math.h>

/* make_dual */
Dual make_dual(float real_part, float dual_part) {
    Dual d;
    d.real = real_part;
    d.dual = dual_part;
    return d;
}

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
    float val = (float)pow(a.real, b.real);
    float deriv = val * (b.dual * log(a.real) + b.real * (a.dual / a.real));
    return make_dual(val, deriv);
}

/* Unary operation */
Dual dual_sin(Dual x) {
    return make_dual((float)sin(x.real), (float)cos(x.real) * x.dual);
}

Dual dual_cos(Dual x) {
    return make_dual((float)cos(x.real), -(float)sin(x.real) * x.dual);
}

Dual dual_tan(Dual x) {
    return make_dual((float)tan(x.real), x.dual / ((float)cos(x.real) * (float)cos(x.real)));
}

Dual dual_exp(Dual x) {
    return make_dual((float)exp(x.real), (float)exp(x.real) * x.dual);
}

Dual dual_log(Dual x) {
    return make_dual((float)log(x.real), x.dual / x.real);
}