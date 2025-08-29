#include "PPM.h"

PPM::~PPM(){
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

void PPM::read(){
    char magic[3];
    cin >> magic;

    if(magic[1] != 'P' || magic[0] != '3'){
        cerr << "Incorrect format." << endl;
        exit(1);
    }

    while(cin.peek() == '#'){
        char tempBuff[256];
        cin.getline(tempBuff, 256);
    }

    cin >> W >> H >> mColor;
    img = new RGB*[H];
    tempImg = new RGB*[H];
    for(int i = 0; i < H; i++){
        img[i] = new RGB[W];
        tempImg[i] = new RGB[W];
    }

    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cin >> img[i][j].r >> img[i][j].g >> img[i][j].b;
        }
    }
}

void PPM::writeImage(){
    cout << "P3\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cout << tempImg[i][j].r << " " << tempImg[i][j].g << " " << tempImg[i][j].b << " ";
        }
        cout << "\n";
    }
}

void PPM::blurFilter(){
    RGB **blurred = new RGB*[H];
    for(int i = 0; i < H; i++){
        blurred[i] = new RGB[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sumR = 0.0, sumG = 0.0, sumB = 0.0;
            float weight_sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sumR += img[ny][nx].r * blur_kernel[ky+1][kx+1];
                        sumG += img[ny][nx].g * blur_kernel[ky+1][kx+1];
                        sumB += img[ny][nx].b * blur_kernel[ky+1][kx+1];
                        weight_sum += blur_kernel[ky+1][kx+1];
                    }
                }
            }
            int resultR = static_cast<int>(sumR / weight_sum);
            int resultG = static_cast<int>(sumG / weight_sum);
            int resultB = static_cast<int>(sumB / weight_sum);
            resultR = max(0, min(mColor, resultR));
            resultG = max(0, min(mColor, resultG));
            resultB = max(0, min(mColor, resultB));

            blurred[y][x].r = resultR;
            blurred[y][x].g = resultG;
            blurred[y][x].b = resultB;
        }
    }
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            tempImg[i][j] = blurred[i][j];
        }
    }
    for(int i = 0; i < H; i++){
        delete[] blurred[i];
    }
    delete[] blurred;
}

void PPM::laplaceFilter(){
    RGB **laplace = new RGB*[H];
    for(int i = 0; i < H; i++){
        laplace[i] = new RGB[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sumR += img[ny][nx].r * laplace_kernel[ky+1][kx+1];
                        sumG += img[ny][nx].g * laplace_kernel[ky+1][kx+1];
                        sumB += img[ny][nx].b * laplace_kernel[ky+1][kx+1];
                    }
                }
            }
            int resultR = static_cast<int>(sumR);
            int resultG = static_cast<int>(sumG);
            int resultB = static_cast<int>(sumB);
            resultR = max(0, min(mColor, resultR));
            resultG = max(0, min(mColor, resultG));
            resultB = max(0, min(mColor, resultB));
            laplace[y][x].r = resultR;
            laplace[y][x].g = resultG;
            laplace[y][x].b = resultB;
        }
    }
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            tempImg[i][j] = laplace[i][j];
        }
    }
    for(int i = 0; i < H; i++){
        delete[] laplace[i];
    }
    delete[] laplace;
}

void PPM::sharpenFilter(){
    RGB **sharpened = new RGB*[H];
    for(int i = 0; i < H; i++){
        sharpened[i] = new RGB[W];
    }

    for(int y = 0; y < H; y++){
        for(int x = 0; x < W; x++){
            float sumR = 0.0, sumG = 0.0, sumB = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sumR += img[ny][nx].r * sharpen_kernel[ky+1][kx+1];
                        sumG += img[ny][nx].g * sharpen_kernel[ky+1][kx+1];
                        sumB += img[ny][nx].b * sharpen_kernel[ky+1][kx+1];
                    }
                }
            }
            int resultR = static_cast<int>(sumR);
            int resultG = static_cast<int>(sumG);
            int resultB = static_cast<int>(sumB);
            resultR = max(0, min(mColor, resultR));
            resultG = max(0, min(mColor, resultG));
            resultB = max(0, min(mColor, resultB));
            sharpened[y][x].r = resultR;
            sharpened[y][x].g = resultG;
            sharpened[y][x].b = resultB;
        }
    }
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            tempImg[i][j] = sharpened[i][j];
        }
    }
    for(int i = 0; i < H; i++){
        delete[] sharpened[i];
    }
    delete[] sharpened;
}