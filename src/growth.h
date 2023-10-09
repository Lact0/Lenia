#pragma once

class GrowthFunc {
    public:
        virtual double growth(double u) = 0;
};

class ExpGF : public GrowthFunc {
    public:
        double mean;
        double sigma;

        ExpGF(double m, double s);
        double growth(double u);
};