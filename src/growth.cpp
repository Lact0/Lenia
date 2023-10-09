#include "growth.h"
#include <cmath>


ExpGF::ExpGF(double m, double s) {
    mean = m;
    sigma = s;
}

double ExpGF::singleGrowth(double u) {
    double num = pow(u - mean, 2);
    double den = 2 * pow(sigma, 2);
    return 2 * exp(-num / den) - 1;
}

vector<double> ExpGF::growth(vector<double> u) {
    vector<double> growths(u.size(), 0);
    for(int i = 0; i < u.size(); i++) {
        growths[i] = singleGrowth(u[i]);
    }
    return growths;
}