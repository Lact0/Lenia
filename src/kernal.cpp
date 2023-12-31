#include "kernal.h"
#include <iostream>
#include <thread>

ExpK::ExpK(int w, int h, double a, double r, vector<double> p) {
    width = w;
    height = h;
    alpha = a;
    radius = r;
    peaks = p;
    kernal = vector<double>(width * height, 0);
    genKernal();
}

vector<double> Kernal::convolve(vector<double> grid) {
    vector<double> conv(grid.size(), 0);
    int numThreads = thread::hardware_concurrency();
    thread threads[numThreads];
    for(int i = 0; i < numThreads; i++) {
        threads[i] = thread(&Kernal::workerConvolve, this, grid, &conv, i, numThreads);
    }
    for(int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
    return conv;
}

void Kernal::workerConvolve(vector<double> grid, vector<double>* conv, int ind, int jmp) {
    int size = grid.size();
    for(int i = ind; i < size; i += jmp) {
        int offset = size - i;
        for(int j = 0; j < size; j++) {
            (*conv)[i] += grid[j] * kernal[(offset + j) & (size - 1)];
        }
    }
}

void ExpK::genKernal() {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            double xChange = std::min(i, width - i);
            double yChange = std::min(j, height - j);
            double dist = sqrt(pow(xChange, 2) + pow(yChange, 2)) / radius;
            if(dist > 1) {
                continue;
            }
            double br = peaks.size() * dist;
            double val = peaks[floor(br)] * shell(fmod(br, 1));
            kernal[i + j * width] = val;
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
    return kernal[shiftedX + shiftedY * width];
}

double ExpK::shell(double dist) {
    double den = 4 * dist * (1 - dist);
    return exp(alpha - alpha/den);
}