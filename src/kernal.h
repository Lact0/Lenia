#pragma once
#include <vector>

using namespace std;

class Kernal {
    virtual vector<double> convolve(vector<double> grid) = 0;
};

class ExpK : public Kernal {
    public:
        int width;
        int height;
        double alpha;
        double radius;
        vector<double> peaks;

        ExpK(int w, int h, double a, double r, vector<double> p);

        vector<double> convolve(vector<double> grid);
        double getKernalPoint(int x, int y, int i, int j);
        double shell(double dist);
    
    private:
        double mag;
        vector<double> kernal;

        void workerConvolve(vector<double> grid, vector<double>* conv, int ind, int jmp);
        void genKernal();
        
};