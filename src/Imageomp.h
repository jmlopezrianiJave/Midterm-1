#ifndef IMAGEOMP_H
#define IMAGEOMP_H

#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

class Imageomp{
    protected:
        int H, W, mColor; //Height, width, max color
    public:
        virtual ~Imageomp() {}
        virtual void read() = 0; //reads image given
        /*virtual void process() = 0; //process it to be able to apply filter*/
        virtual void writeImage() = 0; //writes filtered image to original format
        virtual void blurFilter(const char *outFile = nullptr) = 0; //applies blur filter
        virtual void laplaceFilter(const char *outFile = nullptr) = 0; //applies laplace filter
        virtual void sharpenFilter(const char *outFile = nullptr) = 0; //applies sharpen filter
};

#endif