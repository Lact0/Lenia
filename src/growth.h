#pragma once
#include <vector>

using namespace std;

class GrowthFunc {
    public:
        virtual vector<double> growth(vector<double> u) = 0;
        virtual double singleGrowth(double u) = 0;
};

class ExpGF : public GrowthFunc {
    public:
        double mean;
        double sigma;

        ExpGF(double m, double s);
        vector<double> growth(vector<double> u);
        double singleGrowth(double u);
};