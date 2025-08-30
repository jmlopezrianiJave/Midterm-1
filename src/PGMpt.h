#ifndef PGMPT_H
#define PGMPT_H

#include "Image.h"
#include <pthread.h>

struct FilterArgs{
    PGMpt *pgm;
    int start;
    int end;
    float (*kernel)[3];
    bool normalize;
    int **output;
};

class PGMpt:public Image{
    int **img;
    int **tempImg;

    private:
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
            {-1.0f, -1.0f, -1.0f},
            {-1.0f,  9.0f, -1.0f},
            {-1.0f, -1.0f, -1.0f}
        };

    public:
        ~PGMpt();
        void read() override;
        // void process() override;
        void writeImage() override;
        void blurFilter() override;
        void laplaceFilter() override;
        void sharpenFilter() override;
};

#endif