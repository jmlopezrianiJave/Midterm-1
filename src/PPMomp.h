#ifndef PPMOMP_H
#define PPMOMP_H

#include "Imageomp.h"

struct RGB{
    int r, g, b;
};

class PPMomp:public Imageomp{
    RGB **img;
    RGB **tempImg;

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
        ~PPMomp();
        void read() override;
        void writeImage() override;
        void blurFilter(const char *outFile = nullptr) override;
        void laplaceFilter(const char *outFile = nullptr) override;
        void sharpenFilter(const char *outFile = nullptr) override;
};

#endif