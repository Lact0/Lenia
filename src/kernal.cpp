#include "kernal.h"
#include <cmath>
#include <iostream>

ExpK::ExpK(int w, int h, double a, double r, vector<double> p) {
    width = w;
    height = h;
    alpha = a;
    radius = r;
    peaks = p;
    kernal = vector<double>(width * height, 0);
    genKernal();
}

vector<double> ExpK::convolve(vector<double> grid) {
    vector<double> convolution;
    return convolution;
}

void ExpK::genKernal() {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < width; j++) {
            double xChange = std::min(i, width - i);
            double yChange = std::min(j, height - j);
            double dist = sqrt(pow(xChange, 2) + pow(yChange, 2)) / radius;
            if(dist > 1) {
                continue;
            }
            double br = peaks.size() * dist;
            double val = peaks[floor(br)] * shell(fmod(br, 1));
            kernal[i * width + j] = val;
            mag += val;
        }
    }
    for(int i = 0; i < kernal.size(); i++) {
        kernal[i] /= mag;
    }
}

double ExpK::getKernalPoint(int x, int y, int i, int j) {
    int shiftedX = (i - x + width) % width;
    int shiftedY = (j - y + height) % height;
    return kernal[shiftedX * width + shiftedY];
}

double ExpK::shell(double dist) {
    double den = 4 * dist * (1 - dist);
    return exp(alpha - alpha/den);
}