#include "PGMomp.h"

PGMomp::~PGMomp(){
    if(img){
        for(int i = 0; i < H; i++){
            delete[] img[i];
        }
        delete[] img;
    }
    if(tempImg){
        for(int i = 0; i < H; i++){
            delete[] tempImg[i];
        }
        delete[] tempImg;
    }
}

void PGMomp::read(){
    char magic[3];
    cin >> magic;

    if(magic[1] != 'P' || magic[0] != '2'){
        cerr << "Incorrect format." << endl;
        exit(1);
    }

    while(cin.peek() == '#'){
        char tempBuff[256];
        cin.getline(tempBuff, 256);
    }

    cin >> W >> H >> mColor;
    img = new int*[H];
    tempImg = new int*[H];
    for(int i = 0; i < H; i++){
        img[i] = new int[W];
        tempImg[i] = new int[W];
    }

    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cin >> img[i][j];
        }
    }
}

void PGMomp::writeImage(){
    cout << "P2\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cout << tempImg[i][j] << "\n";
        }
        cout << "\n";
    }
}

void PGMomp::blurFilter(const char *outFile){
    int **blurred = new int*[H];
    for(int i = 0; i < H; i++){
        blurred[i] = new int[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sum = 0.0;
            float weight_sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sum += img[ny][nx] * blur_kernel[ky+1][kx+1];
                        weight_sum += blur_kernel[ky+1][kx+1];
                    }
                }
            }
            int result = static_cast<int>(sum / weight_sum);
            result = max(0, min(mColor, result));
            blurred[y][x] = result;
        }
    }
    ofstream out(outFile);
    out << "P2\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i  < H; ++i){
        for(int j = 0; j < W; ++j){
            out << blurred[i][j] << "\n";
        }
        out << "\n";
    }
    out.close();
    for(int i = 0; i < H; i++){
        delete[] blurred[i];
    }
    delete[] blurred;
}

void PGMomp::laplaceFilter(const char *outFile){
    int **laplace = new int*[H];
    for(int i = 0; i < H; i++){
        laplace[i] = new int[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sum += img[ny][nx] * laplace_kernel[ky+1][kx+1];
                    }
                }
            }
            int result = static_cast<int>(sum);
            result = max(0, min(mColor, result));
            laplace[y][x] = result;
        }
    }
    ofstream out(outFile);
    out << "P2\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i  < H; ++i){
        for(int j = 0; j < W; ++j){
            out << laplace[i][j] << "\n";
        }
        out << "\n";
    }
    out.close();
    for(int i = 0; i < H; i++){
        delete[] laplace[i];
    }
    delete[] laplace;
}

void PGMomp::sharpenFilter(const char *outFile){
    int **sharpened = new int*[H];
    for(int i = 0; i < H; i++){
        sharpened[i] = new int[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sum += img[ny][nx] * sharpen_kernel[ky+1][kx+1];
                    }
                }
            }
            int result = static_cast<int>(sum);
            result = max(0, min(mColor, result));
            sharpened[y][x] = result;
        }
    }
    ofstream out(outFile);
    out << "P2\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i  < H; ++i){
        for(int j = 0; j < W; ++j){
            out << sharpened[i][j] << "\n";
        }
        out << "\n";
    }
    out.close();
    for(int i = 0; i < H; i++){
        delete[] sharpened[i];
    }
    delete[] sharpened;
}