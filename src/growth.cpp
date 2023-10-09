#include "growth.h"
#include <cmath>

ExpGF::ExpGF(double m, double s) {
    mean = m;
    sigma = s;
}

double ExpGF::growth(double u) {
    double num = pow(u - mean, 2);
    double den = 2 * pow(sigma, 2);
    return 2 * exp(-num / den) - 1;
}