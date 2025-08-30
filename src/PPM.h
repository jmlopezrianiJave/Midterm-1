#ifndef PPM_H
#define PPM_H

#include "Image.h"

struct RGB{
    int r, g, b;
};

class PPM:public Image{
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
        ~PPM();
        void read() override;
        void writeImage() override;
        void blurFilter() override;
        void laplaceFilter() override;
        void sharpenFilter() override;
};

#endif