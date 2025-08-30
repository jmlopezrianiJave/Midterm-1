#include "PGMpt.h"

PGMpt::~PGMpt(){
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

void PGMpt::read(){
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

void PGMpt::writeImage(){
    cout << "P2\n" << W << " " << H << "\n" << mColor << "\n";
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            cout << tempImg[i][j] << "\n";
        }
        cout << "\n";
    }
}

static void *applyFilterWorker(void *vargs){
    FilterArgs *args = (FilterArgs*)vargs;
    PGMpt * p = args->pgm;
    int W = p->W;
    int H = p->H;
    int mColor = p->mColor;
    int **img = p->img;
    int**out = args->output;
    float(*kernel)[3] = args->kernel;
    bool normalize = args->normalize;

    int **blurred = new int*[H];
    for(int i = 0; i < H; i++){
        blurred[i] = new int[W];
    }

    for(int y = args->start; y < args->end; y++){
        for(int x = 0; x < W; x++){
            float sum = 0.0;
            float weight_sum = 0.0;
            for(int ky = -1; ky <= 1; ky++){
                for(int kx = -1; kx <= 1; kx++){
                    int nx = x + kx;
                    int ny = y + ky;
                    if(nx >= 0 && nx < W && ny >= 0 && ny < H){
                        sum += img[ny][nx] * kernel[ky+1][kx+1];
                        weight_sum += kernel[ky+1][kx+1];
                    }
                }
            }
            int result;
            if(normalize){
                if(weight_sum == 0.0) result = static_cast<int>(sum);
                else result = static_cast<int>(sum / weight_sum);
            } else {
                result = static_cast<int>(sum);
            }
            result = max(0, min(mColor, result));
            out[y][x] = result;
        }
    }
    return nullptr;
}

static void applyFilter(PGMpt *p, const float kernel_in[3][3], bool normalize){
    int num_threads = 4;

    pthread_t threads[4];
    FilterArgs args[4];

    int base = p->H / num_threads;
    int rem = p->H % num_threads;
    int cur = 0;

    for(int i = 0; i < num_threads; ++i){
        int rows = base + (i < rem ? 1 : 0);
        args[i].pgm = p;
        args[i].start = cur;
        args[i].end = cur + rows;
        args[i].kernel = const_cast<float(*)[3](kernel_in);
        args[i].normalize = normalize;
        args[i].output = p->tempImg;
        pthread_create(&threads[i], nullptr, applyFilterWorker, &args[i]);
        cur += rows;
    }

    for(int j = 0; j < num_threads; ++j) pthread_join(threads[t], nullptr);
}

/* multithreaded versions (row-splitting) */
void PGM::blurFilter(int num_threads){
    // blur uses normalization (divide by sum of weights of valid neighbors)
    applyFilter(this, blur_kernel, true, num_threads);
}

void PGM::laplaceFilter(int num_threads){
    // laplace does not normalize
    applyFilter(this, laplace_kernel, false, num_threads);
}

void PGM::sharpenFilter(int num_threads){
    // sharpen does not normalize
    applyFilter(this, sharpen_kernel, false, num_threads);
}