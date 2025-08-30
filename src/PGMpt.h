#ifndef PGMPT_H
#define PGMPT_H

#include "Image.h"
#include <pthread.h>

class PGMpt;

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
        ~PGMpt();
        void read() override;
        // void process() override;
        void writeImage() override;
        void blurFilter() override;
        void laplaceFilter() override;
        void sharpenFilter() override;
};

#endif