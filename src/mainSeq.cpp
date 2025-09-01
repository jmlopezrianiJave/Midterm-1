#include "PGM.h"
#include "PPM.h"
#include <cstring>
#include <iostream>
#include <chrono> 
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
    auto start = chrono::high_resolution_clock::now();  

    Image* image = nullptr;
    freopen(argv[1], "r", stdin);
    string filter = argv[2];
    string title = baseName(argv[1]);

    char magicNum[3];
    cin >> magicNum;

    for(int i = 0; i < (int)strlen(magicNum); i++){
        cin.putback(magicNum[i]);
    }

    if(magicNum[0] == 'P' && magicNum[1] == '2'){
        image = new PGM();
        image->read();
        if(filter == "b"){
            //blur
            string outFile = "images/" + title + "_blur.pgm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->blurFilter();
            image->writeImage();
            fclose(out);  
        }
        else if(filter == "l"){
            //laplace
            string outFile = "images/" + title + "_laplace.pgm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->laplaceFilter();
            image->writeImage();
            fclose(out);
        }
        else if(filter == "s"){
            //SHARPEN
            string outFile = "images/" + title + "_sharpen.pgm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->sharpenFilter();
            image->writeImage();
            fclose(out);
        }
    }
    else if(magicNum[0] == 'P' && magicNum[1] == '3'){
        image = new PPM();
        image->read();
        if(filter == "b"){
            //blur
            string outFile = "images/" + title + "_blur.ppm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->blurFilter();
            image->writeImage();
            fclose(out);
        }
        else if(filter == "l"){
            //laplace
            string outFile = "images/" + title + "_laplace.ppm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->laplaceFilter();
            image->writeImage();
            fclose(out);
        }
        else if(filter == "s"){
            //SHARPEN
            string outFile = "images/" + title + "_sharpen.ppm";
            FILE* out = freopen(outFile.c_str(), "w", stdout);
            image->sharpenFilter();
            image->writeImage();
            fclose(out);
        }
    }
    else{
        cout << magicNum << endl;
        cerr << "Wrong format." << endl;
        return 1;
    }

    auto end = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cerr << "Wall time: " << ms << " ms" << endl;

    return 0;
}
