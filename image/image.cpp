//#include <cstdint>
//#include "image.h"
#include <algorithm>
#include <iostream>
class Image {
private:
    int width;         // Width of the image
    int height;        // Height of the image
    int bufferLength;  // Total length of the pixel buffer (width * height * 4)
    uint8_t* buffer; // Buffer to hold RGBA pixel data (8 bits per channel)

public:
    // Constructor
    Image(int w, int h) : width(w), height(h) {
        bufferLength = width * height * 4;   // 4 bytes per pixel (R, G, B, A)
        buffer = new uint8_t[bufferLength]; // Dynamically allocate buffer
    }

    // Destructor to release memory
    ~Image() {
        delete[] buffer;
    }

    typedef struct {
        uint32_t R[2];
        uint32_t G[2];
        uint32_t B[2];
    } ImageHistogram;

    void initHistogram(ImageHistogram *hist) {
        hist->R[0] = 0;
        hist->R[1] = 0;
        hist->G[0] = 0;
        hist->G[1] = 0;
        hist->B[0] = 0;
        hist->B[1] = 0;
    }

    void updateHistogram(ImageHistogram *hist, uint8_t r, uint8_t g, uint8_t b) {
        hist->R[r > 127]++;  // If r > 127, index is 1, else 0
        hist->G[g > 127]++;  // If g > 127, index is 1, else 0
        hist->B[b > 127]++;  // If b > 127, index is 1, else 0
    }
    // Method to set pixel at (x, y) to specific R, G, B, A values
    void setPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
        int index = (y * width + x) * 4;     // Calculate position in the buffer
        buffer[index] = R;                   // Set Red value
        buffer[index + 1] = G;               // Set Green value
        buffer[index + 2] = B;               // Set Blue value
        buffer[index + 3] = A;               // Set Alpha value
    }

    // Method to print all pixels in the image
    void print() const {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = (y * width + x) * 4;  // Calculate position in the buffer
                uint8_t R = buffer[index];       // Get Red value
                uint8_t G = buffer[index + 1];   // Get Green value
                uint8_t B = buffer[index + 2];   // Get Blue value
                uint8_t A = buffer[index + 3];   // Get Alpha value

                std::cout << "Pixel (" << x << ", " << y << ") - R: "
                          << static_cast<int>(R) << ", G: "
                          << static_cast<int>(G) << ", B: "
                          << static_cast<int>(B) << ", A: "
                          << static_cast<int>(A) << std::endl;
            }
        }
    }

    // Method to calculate the histogram of RGBA values
    void calculateHistogram()  {
        // Histograms for each channel
        ImageHistogram* hist = (ImageHistogram*) malloc(sizeof(ImageHistogram));
        initHistogram(hist);
        // Traverse the buffer to calculate histogram
        for (int i = 0; i < bufferLength; i += 4) {
            uint8_t R = static_cast<uint8_t>(buffer[i]);       // Get Red value
            uint8_t G = static_cast<uint8_t>(buffer[i + 1]);   // Get Green value
            uint8_t B = static_cast<uint8_t>(buffer[i + 2]);   // Get Blue value

            // Add pixel values to corresponding histogram
            updateHistogram(hist,R,G,B);//, uint8_t r, uint8_t g, uint8_t b)
        }
        std::cout << "hist of R[0] "  << hist->R[0] << " r[1] " << hist->R[1] << "\n";
        std::cout << "hist of G[0] "  << hist->G[0] << " g[1] " << hist->G[1] << "\n";
        std::cout << "hist of B[0] "  << hist->B[0] << " b[1] " << hist->B[1] << "\n";
        
    }

    void downsample( int factor, uint8_t* output ) {
        int out_width = (width + factor - 1 ) / factor;
        int out_height = (height + factor - 1) / factor;

        int block_size = factor * factor;

        for (int row = 0; row < out_height; row++){
            for(int col  = 0; col < out_width; col++){

                int sum_r = 0,sum_g = 0, sum_b = 0, sum_a = 0;
                int count = 0;

                for (int block_row = 0; block_row < factor;block_row++){
                    for(int block_col = 0; block_col < factor;block_col++){
                        int orig_row = row * factor + block_row;
                        int orig_col = col * factor + block_col;

                        if(orig_col< width && orig_row < height){
                            int buf_idx = (orig_row * width + orig_col ) * 4;
                            sum_r += buffer[buf_idx];
                            sum_g += buffer[buf_idx + 1];
                            sum_b += buffer[buf_idx + 2];
                            sum_a += buffer[buf_idx + 3];
                            ++count;
                        }
                    }
                }
                uint8_t avg_r = sum_r / count;
                uint8_t avg_g = sum_g / count;
                uint8_t avg_b = sum_b / count;
                uint8_t avg_a = sum_a / count;
                int out_idx = (row * out_width + col) * 4;
                output[out_idx] = avg_r;
                output[out_idx + 1] = avg_g;
                output[out_idx + 2] = avg_b;
                output[out_idx + 3] = avg_a;
            }
        }
    }
    

    //More memory effienciy
    void downsampleV2( int factor, uint8_t* output){
        int out_width = (width + factor - 1 ) / factor;
        int out_height = (height + factor - 1) / factor;
        int count;
        int block_size = factor * factor;
        int block_width = factor;
        unsigned int row, col, sum_r, sum_g, sum_b, sum_a;

        unsigned int block_left = 0, block_top = 0, block_right = 0, block_bottom = 0;

        for(block_left = 0; block_left < width;block_left += block_width){
            block_right = block_left + block_width - 1;
            if(block_right + 1 > width){
                block_right = width - 1;
            }
            for(block_top = 0;block_top < height;block_top += block_width){
                block_bottom = block_top + block_width - 1;
                if(block_bottom + 1 > height){
                    block_bottom = height - 1;                
                }
                sum_r = sum_b = sum_g = 0;
                count = 0; // cnt of pixels in each block size
                for(row = block_top ;row <= block_bottom;row++){
                    for(col = block_left; col <= block_right;col++){
                        int buf_idx = (row * width + col) * 4;
                        sum_r += buffer[buf_idx];
                        sum_g += buffer[buf_idx + 1];
                        sum_b += buffer[buf_idx + 2];
                        sum_a += buffer[buf_idx + 3];
                        ++count;
                    }
                }
                count = std::max(1, count);
                uint8_t avg_r = sum_r / count;
                uint8_t avg_g = sum_g / count;
                uint8_t avg_b = sum_b / count;
                uint8_t avg_a = sum_a / count; 

                int out_row = (block_top + factor - 1) / factor;
                int out_col = (block_left  + factor - 1)/ factor;
                int out_idx = (out_row * out_width + out_col) * 4;
                output[out_idx] = avg_r;
                output[out_idx + 1] = avg_g;
                output[out_idx + 2] = avg_b;
                output[out_idx + 3] = avg_a;               
            }
        } 
    }

    void maxpooling( int factor, uint8_t* output){
        int out_width = (width + factor - 1 ) / factor;
        int out_height = (height  + factor - 1)/ factor;

        int block_size = factor * factor;

        for (int row = 0; row < out_height; row++){
            for(int col  = 0; col < out_width; col++){

                int max_r= 0,max_g = 0, max_b = 0, max_a = 0;

                for (int block_row = 0; block_row < factor;block_row++){
                    for(int block_col = 0; block_col < factor;block_col++){
                        int orig_row = row * factor + block_row;
                        int orig_col = col * factor + block_col;

                        if(orig_col< width && orig_row < height){
                            int buf_idx = (orig_row * width + orig_col ) * 4;
                            max_r = std::max(max_r, static_cast<int>(buffer[buf_idx ]));
                            max_g = std::max(max_g, static_cast<int>(buffer[buf_idx + 1]));
                            max_b = std::max(max_b, static_cast<int>(buffer[buf_idx + 2]));

                            max_a = std::max(max_a, static_cast<int>(buffer[buf_idx + 3]));

                        }
                    }
                }
                int out_idx = (row * out_width + col) * 4;
                output[out_idx] = max_r;
                output[out_idx + 1] = max_g;
                output[out_idx + 2] = max_b;
                output[out_idx + 3] = max_a;
            }
        }
    }
};



int main() {
    // Create a 5x4 image
    Image img(5, 4);
    
    // Set some arbitrary pixels (R, G, B, A values)
    img.setPixel(0, 0, 1, 150, 200, 255);
    img.setPixel(1, 0, 2, 60, 70, 255);
    img.setPixel(2, 0, 3, 30, 40, 255);
    img.setPixel(3, 0, 4, 120, 200, 255);
    img.setPixel(4, 0, 5, 160, 240, 255);

    img.setPixel(0, 1, 6, 50, 150, 255);
    img.setPixel(1, 1, 7, 90, 60, 255);
    img.setPixel(2, 1, 8, 30, 200, 255);
    img.setPixel(3, 1, 9, 70, 100, 255);
    img.setPixel(4, 1, 10, 180, 210, 255);

    img.setPixel(0, 2, 11, 50, 200, 255);
    img.setPixel(1, 2, 12, 150, 120, 255);
    img.setPixel(2, 2, 13, 200, 220, 255);
    img.setPixel(3, 2, 14, 80, 60, 255);
    img.setPixel(4, 2, 15, 100, 150, 255);

    img.setPixel(0, 3, 16, 30, 40, 255);
    img.setPixel(1, 3, 17, 60, 10, 255);
    img.setPixel(2, 3, 18, 200, 60, 255);
    img.setPixel(3, 3, 19, 50, 50, 255);
    img.setPixel(4, 3, 20, 120, 130, 255);

    // Calculate new dimensions based on downsample factor
    int factor = 2;
    int newWidth = (5 + factor - 1) / factor;
    int newHeight = (4 + factor - 1) / factor;

    // Allocate buffer for downsampled image
    uint8_t* downsampledBuffer = new uint8_t[newWidth * newHeight * 4];  // 4 bytes per pixel (RGBA)

    // Downsample the image
    img.downsample(factor, downsampledBuffer);


    uint8_t* downsampledBuffer2 = new uint8_t[newWidth * newHeight * 4];  // 4 bytes per pixel (RGBA)

    img.downsampleV2(factor, downsampledBuffer2);

    uint8_t* maxpool_buf = new uint8_t[newWidth * newHeight * 4];  // 4 bytes per pixel (RGBA)
    img.maxpooling(factor, maxpool_buf);
    // Print downsampled image pixels
    std::cout << "\nDownsampled Image:\n";
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int index = (y * newWidth + x) * 4;
            uint8_t R = downsampledBuffer[index];
            uint8_t G = downsampledBuffer[index + 1];
            uint8_t B = downsampledBuffer[index + 2];
            uint8_t A = downsampledBuffer[index + 3];

            std::cout << "Pixel (" << x << ", " << y << ") - R: "
                      << static_cast<int>(R) << ", G: "
                      << static_cast<int>(G) << ", B: "
                      << static_cast<int>(B) << ", A: "
                      << static_cast<int>(A) << std::endl;
        }
    }

    // Print downsampled image pixels
    std::cout << "\nDownsampled Image V2:\n";
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int index = (y * newWidth + x) * 4;
            uint8_t R = downsampledBuffer2[index];
            uint8_t G = downsampledBuffer2[index + 1];
            uint8_t B = downsampledBuffer2[index + 2];
            uint8_t A = downsampledBuffer2[index + 3];

            std::cout << "Pixel (" << x << ", " << y << ") - R: "
                      << static_cast<int>(R) << ", G: "
                      << static_cast<int>(G) << ", B: "
                      << static_cast<int>(B) << ", A: "
                      << static_cast<int>(A) << std::endl;
        }
    }


    // Print downsampled image pixels
    std::cout << "\nmax pool :\n";
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            int index = (y * newWidth + x) * 4;
            uint8_t R = maxpool_buf[index];
            uint8_t G = maxpool_buf[index + 1];
            uint8_t B = maxpool_buf[index + 2];
            uint8_t A = maxpool_buf[index + 3];

            std::cout << "Pixel (" << x << ", " << y << ") - R: "
                      << static_cast<int>(R) << ", G: "
                      << static_cast<int>(G) << ", B: "
                      << static_cast<int>(B) << ", A: "
                      << static_cast<int>(A) << std::endl;
        }
    }

    img.calculateHistogram();

    // Clean up the dynamically allocated buffer
    delete[] downsampledBuffer;
    delete[] downsampledBuffer2;
    return 0;
}
