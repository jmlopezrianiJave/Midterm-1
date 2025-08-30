#include "PPMpt.h"

PPMpt::~PPMpt(){
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

void PPMpt::read(){
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

void PPMpt::writeImage(){
    cout << "P3\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cout << tempImg[i][j].r << " " << tempImg[i][j].g << " " << tempImg[i][j].b << " ";
        }
        cout << "\n";
    }
}

void *PPMpt::applyFilterWorker(void *vargs){
    FilterArgsPPM *args = (FilterArgsPPM*)vargs;
    PPMpt * p = args->pgm;
    int W = p->W;
    int H = p->H;
    int mColor = p->mColor;
    RGB **img = p->img;
    RGB**out = args->output;
    float(*kernel)[3] = args->kernel;
    bool normalize = args->normalize;

    for(int y = args->start; y < args->end; y++){
        for(int x = 0; x < W; x++){
            float sumR = 0.0, sumG = 0.0, sumB = 0.0;
            float weight_sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sumR += img[ny][nx].r * kernel[ky+1][kx+1];
                        sumG += img[ny][nx].g * kernel[ky+1][kx+1];
                        sumB += img[ny][nx].b * kernel[ky+1][kx+1];
                        weight_sum += kernel[ky+1][kx+1];
                    }
                }
            }
            int resultR, resultG, resultB;
            if(normalize){
                if(weight_sum == 0.0) {
                    resultR = static_cast<int>(sumR);
                    resultG = static_cast<int>(sumG);
                    resultB = static_cast<int>(sumB);
                }
                else{
                    resultR = static_cast<int>(sumR / weight_sum);
                    resultG = static_cast<int>(sumG / weight_sum);
                    resultB = static_cast<int>(sumB / weight_sum);
                }
            } else {
                resultR = static_cast<int>(sumR);
                resultG = static_cast<int>(sumG);
                resultB = static_cast<int>(sumB);
            }
            resultR = max(0, min(mColor, resultR));
            resultG = max(0, min(mColor, resultG));
            resultB = max(0, min(mColor, resultB));

            out[y][x].r = resultR;
            out[y][x].g = resultG;
            out[y][x].b = resultB;
        }
    }
    return nullptr;
}

void PPMpt::applyFilter(const float kernel_in[3][3], bool normalize){
    int num_threads = 4;

    pthread_t threads[4];
    FilterArgsPPM args[4];

    int base = H / num_threads;
    int rem = H % num_threads;
    int cur = 0;

    for(int i = 0; i < num_threads; ++i){
        int rows = base + (i < rem ? 1 : 0);
        args[i].pgm = this;
        args[i].start = cur;
        args[i].end = cur + rows;
        args[i].kernel = const_cast<float(*)[3]>(kernel_in);
        args[i].normalize = normalize;
        args[i].output = tempImg;
        pthread_create(&threads[i], nullptr, applyFilterWorker, &args[i]);
        cur += rows;
    }

    for(int j = 0; j < num_threads; ++j) pthread_join(threads[j], nullptr);
}

/* multithreaded versions (row-splitting) */
void PPMpt::blurFilter(){
    // blur uses normalization (divide by sum of weights of valid neighbors)
    applyFilter(blur_kernel, true);
}

void PPMpt::laplaceFilter(){
    // laplace does not normalize
    applyFilter(laplace_kernel, false);
}

void PPMpt::sharpenFilter(){
    // sharpen does not normalize
    applyFilter(sharpen_kernel, false);
}