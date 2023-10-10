#pragma once
#include <vector>
#include <cmath>
#include <random>

using namespace std;

class Kernal {
    public:
        int width;
        int height;
        double radius;
        vector<double> peaks;
        vector<double> kernal;

        vector<double> convolve(vector<double> grid);
        virtual double shell(double dist) = 0;

    private:

        void workerConvolve(vector<double> grid, vector<double>* conv, int ind, int jmp);
};

class ExpK : public Kernal {
    public:
        double alpha;

        ExpK(int w, int h, double a, double r, vector<double> p);

        double getKernalPoint(int x, int y, int i, int j);
        double shell(double dist);
    
    private:
        double mag;

        void genKernal();
        
};