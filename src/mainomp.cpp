#include "PGMomp.h"
// #include "PPMomp.h"
#include <cstring>
#include <iostream>
#include <omp.h>
using namespace std;

string baseName(const char* path) {
    string s(path);
    size_t pos = s.find_last_of("/\\");
    if (pos != string::npos) s = s.substr(pos+1);
    pos = s.find_last_of('.');
    if (pos != string::npos) s = s.substr(0, pos);
    return s;
}

int main(int argc, char *argv[]){
    Imageomp* image = nullptr;
    freopen(argv[1], "r", stdin);
    string title = baseName(argv[1]);
    char magicNum[3];
    cin >> magicNum;

    for(int i = 0; i < (int)strlen(magicNum); i++){
        cin.putback(magicNum[i]);
    }

    if(magicNum[0] == 'P' && magicNum[1] == '2'){
        image = new PGMomp();
        image->read();
        PGMomp *p = static_cast<PGMomp *>(image);
        string outBlur  = "images/" + title + "_bluromp.pgm";
        string outLap   = "images/" + title + "_laplaceomp.pgm";
        string outSharp = "images/" + title + "_sharpenomp.pgm";

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                p->blurFilter(outBlur.c_str());
            }

            #pragma omp section
            {
                p->laplaceFilter(outLap.c_str());
            }

            #pragma omp section
            {
                p->sharpenFilter(outSharp.c_str());
            }
        }
    }
    // else if(magicNum[0] == 'P' && magicNum[1] == '3'){
    //     image = new PPM();
    //     image->read();

    //     //blur
    //     string outFile = "images/" + title + "_blur.ppm";
    //     FILE* out = freopen(outFile.c_str(), "w", stdout);
    //     image->blurFilter();
    //     image->writeImage();
    //     fclose(out);

    //     //laplace
    //     outFile = "images/" + title + "_laplace.ppm";
    //     out = freopen(outFile.c_str(), "w", stdout);
    //     image->laplaceFilter();
    //     image->writeImage();
    //     fclose(out);

    //     //SHARPEN
    //     outFile = "images/" + title + "_sharpen.ppm";
    //     out = freopen(outFile.c_str(), "w", stdout);
    //     image->sharpenFilter();
    //     image->writeImage();
    //     fclose(out);
    // }
    else{
        cout << magicNum << endl;
        cerr << "Wrong format." << endl;
        return 1;
    }
}