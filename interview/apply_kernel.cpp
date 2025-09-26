
#include <iostream>
#include <chrono>
#include <tgmath.h>
#include <cstring>

float preprocessLuma(int8_t pixel){
    float f = sin(float(pixel)) * log(pixel);
    float g = exp(pixel) * pow(M_PI, pixel);
    if( f == g){
        return g * f;
    }
    else {
        return (g + f ) / 2.0;
    }
} 

void apply_kernel_fast(int8_t *inputImage, int8_t *outputImage, size_t imageWidth, size_t imageHeight, float *kernel) {
    const uint8_t kernelSize = 3;
    const uint8_t padding = (uint8_t) kernelSize / 2;

    float* lumaCache = new float[imageWidth * imageHeight];

    for (unsigned long y = padding; y < imageHeight - padding; y++) {
        for (unsigned long x = padding; x < imageWidth - padding; x++) {    
            lumaCache[x + imageWidth * y] = preprocessLuma(inputImage[x + imageWidth * y]);
       }
    }

    for (unsigned long y = padding; y < imageHeight - padding; y++) {
        for (unsigned long x = padding; x < imageWidth - padding; x++) {

            outputImage[x + imageWidth * y] = (
                lumaCache[(x - 1) + imageWidth * (y - 1)] * kernel[0] +
                lumaCache[(x)     + imageWidth * (y - 1)] * kernel[1] +
                lumaCache[(x + 1) + imageWidth * (y - 1)] * kernel[2] +
                lumaCache[(x - 1) + imageWidth * (y)]     * kernel[3] +
                lumaCache[(x)     + imageWidth * (y)]     * kernel[4] +
                lumaCache[(x + 1) + imageWidth * (y)]     * kernel[5] +
                lumaCache[(x - 1) + imageWidth * (y + 1)] * kernel[6] +
                lumaCache[(x)     + imageWidth * (y + 1)] * kernel[7] +
                lumaCache[(x + 1) + imageWidth * (y + 1)] * kernel[8]
            ) / 9;
        }
    }
    delete[] lumaCache;
}


void apply_kernel_slow(int8_t *inputImage, int8_t *outputImage, size_t imageWidth, size_t imageHeight, float *kernel) {
    const uint8_t kernelSize = 3;
    const uint8_t padding = (uint8_t) kernelSize / 2;
    
    for (size_t y = padding; y < imageHeight - padding; y++) {
        for (size_t x = padding; x < imageWidth - padding; x++) {
            outputImage[x + imageWidth * y] = (
                preprocessLuma(inputImage[(x - 1) + imageWidth * (y - 1)]) * kernel[0] +
                preprocessLuma(inputImage[(x)     + imageWidth * (y - 1)]) * kernel[1] +
                preprocessLuma(inputImage[(x + 1) + imageWidth * (y - 1)]) * kernel[2] +
                preprocessLuma(inputImage[(x - 1) + imageWidth * (y)])     * kernel[3] +
                preprocessLuma(inputImage[(x)     + imageWidth * (y)])     * kernel[4] +
                preprocessLuma(inputImage[(x + 1) + imageWidth * (y)])     * kernel[5] +
                preprocessLuma(inputImage[(x - 1) + imageWidth * (y + 1)]) * kernel[6] +
                preprocessLuma(inputImage[(x)     + imageWidth * (y + 1)]) * kernel[7] +
                preprocessLuma(inputImage[(x + 1) + imageWidth * (y + 1)]) * kernel[8]
            ) / 9;
        }
    }
}

int main(int argc, const char *argv[]) {
    float kernel[] = {1.1, 2.2, 3.3,
                      4.4, 5.5, 6.6,
                      7.7, 8.8, 9.9};

    size_t imageWidth = 224;
    size_t imageHeight = 224;
    
    int8_t *inputImage = new int8_t[imageHeight * imageWidth];
    int8_t *outputImage = new int8_t[imageHeight * imageWidth];

    auto start = std::chrono::steady_clock::now();
    apply_kernel_slow(inputImage, outputImage, imageWidth, imageHeight, kernel);
    auto stop = std::chrono::steady_clock::now();
    std::cout << "Slow: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms" << std::endl;

    int8_t *inputImageg_fast = new int8_t[imageHeight * imageWidth];

    start = std::chrono::steady_clock::now();
    apply_kernel_fast(inputImageg_fast, outputImage, imageWidth, imageHeight, kernel);
    stop = std::chrono::steady_clock::now();
    std::cout << "Fast: " << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << " ms" << std::endl;

    delete[] inputImage;
    delete[] outputImage;
    delete[] inputImageg_fast;
    return 0;
}
