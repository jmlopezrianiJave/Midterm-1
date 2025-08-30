#ifndef PPMPT_H
#define PPMPT_H

#include "Image.h"
#include <pthread.h>

class PPMpt;

struct RGB{
    int r, g, b;
};

struct FilterArgsPPM{
    PPMpt *pgm;
    int start;
    int end;
    float (*kernel)[3];
    bool normalize;
    RGB **output;
};

class PPMpt:public Image{
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
    
    private:
        static void *applyFilterWorker(void *vargs);
        void applyFilter(const float kernel_in[3][3], bool normalize);

    public:
        ~PPMpt();
        void read() override;
        void writeImage() override;
        void blurFilter() override;
        void laplaceFilter() override;
        void sharpenFilter() override;
};

#endif