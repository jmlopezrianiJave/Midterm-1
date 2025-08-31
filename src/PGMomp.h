#ifndef PGMOMP_H
#define PGMOMP_H

#include "Imageomp.h"

class PGMomp:public Imageomp{
    int **img;
    int **tempImg;

    const float blur_kernel[3][3] = {
        {1.0/9, 1.0/9, 1.0/9},
        {1.0/9, 1.0/9, 1.0/9},
        {1.0/9, 1.0/9, 1.0/9}
    };

    const float laplace_kernel[3][3] = {
        {0.0, 1.0, 0.0},
        {1.0,  -4.0, 1.0},
        {0.0, 1.0, 0.0}
    };

    const float sharpen_kernel[3][3] = {
        {0.0, -1.0, 0.0},
        {-1.0,  5.0, -1.0},
        {0.0, -1.0, 0.0}
    };

    public:
        ~PGMomp();
        void read() override;
        // void process() override;
        void writeImage() override;
        void blurFilter(const char *outFile = nullptr) override;
        void laplaceFilter(const char *outFile = nullptr) override;
        void sharpenFilter(const char *outFile = nullptr) override;
};

#endif