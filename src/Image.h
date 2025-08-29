#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstdlib>
using namespace std;

class Image{
    protected:
        int H, W, mColor; //Height, width, max color
    public:
        virtual ~Image() {}
        virtual void read() = 0; //reads image given
        /*virtual void process() = 0; //process it to be able to apply filter*/
        virtual void writeImage() = 0; //writes filtered image to original format
        virtual void blurFilter() = 0; //applies blur filter
        virtual void laplaceFilter() = 0; //applies laplace filter
        virtual void sharpenFilter() = 0; //applies sharpen filter
};

#endif