#include <iostream>
#include <vector>
#include <cmath>
#include <cstring> // For memset

class GrayscaleImage {
private:
    int width;
    int height;
    uint8_t* buffer;
    std::vector<std::vector<uint8_t> > imageIn2d;

public:
    // Constructor
    GrayscaleImage(int w, int h) : width(w), height(h) {
        buffer = new uint8_t[width * height];
        imageIn2d = std::vector<std::vector<uint8_t> >(height, std::vector<uint8_t>(width, 0)); 

    }
    
    // Destructor
    ~GrayscaleImage() {
        delete[] buffer;
    }

    // Method to set pixel value
    void setPixel(int x, int y, uint8_t value) {
        buffer[y * width + x] = value;
    }

    // Method to get pixel value
    uint8_t getPixel(int x, int y) const {
        return buffer[y * width + x];
    }

    // Method to get pixel value with boundary check (for padding)
    uint8_t getPixelWithPadding(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return 0;  // Return 0 for padding areas
        }
        return getPixel(x, y);
    }

    // Method to apply 2D convolution with zero-padding
    void applyConvolutionWithPadding(const int8_t kernel[3][3]) {
        size_t kernelSize = sizeof(kernel[0]) / sizeof(kernel[0][0]);
        int offset = kernelSize / 2; // For a 5x5 kernel, offset is 2

        // Create a buffer for the new image
        uint8_t* newBuffer = new uint8_t[width * height];

        // Apply convolution with zero-padding
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint8_t newValue = 0;
                for (int ky = -offset; ky <= offset; ++ky) {
                    for (int kx = -offset; kx <= offset; ++kx) {
                        int imageX = x + kx;
                        int imageY = y + ky;
                        newValue += getPixelWithPadding(imageX, imageY) * kernel[ky + offset][kx + offset];
                    }
                }
                //Store convolution result in new buffer
                newBuffer[y * width + x] = newValue;
            }
        }

        // Copy the result from the temporary buffer to the original buffer
        std::memcpy(buffer, newBuffer, width * height * sizeof(uint8_t));

        // Clean up temporary buffer
        delete[] newBuffer;
    }


    // Method to print the image
    void print() const {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                std::cout << static_cast<int>(getPixel(x, y)) << ' ';
            }
            std::cout << std::endl;
        }
    }
};

//Following is representation of image 2D vector. May not actually represent the most efficient
// image represnetation but for practicie purpose
static int getPixelWithPaddingFromInput(int x, int y, int w, int h, const std::vector<std::vector<int> >& input) {
    if (x < 0 || x >= w || y < 0 || y >= h) {
        return 0;  // Return 0 for padding areas
    }
    return input[y][x];
}

static void convolutionWithStride(const std::vector<std::vector<int> >& input,
                            const std::vector<std::vector<int> >& kernel,
                            std::vector<std::vector<int> >& output,
                            int stride)
{
    int output_width = 0;
    int in_row = input.size();
    int in_col = input[0].size();
    int padding = kernel[0].size() / 2;
    int k = kernel.size();

    int offset = kernel.size() / 2;
    // output dimension with stride calculation : https://www.appsilon.com/post/convolutional-neural-networks
    int out_row = (in_row - k + 2 * padding) / stride + 1;
    int out_col = (in_col - k + 2 * padding) / stride + 1;
    output.resize(out_row, std::vector<int>(out_col, 0));

    for(int y = 0; y < out_row;y++){
        for(int x = 0; x < out_col;x++){
            int sum = 0;

            for(int ky = -1 * offset;ky <= offset;ky++){
                for(int kx = -1 * offset;kx <= offset;kx++) {
                    int in_y = y * stride + ky;
                    int in_x = x * stride + kx;
                    sum += getPixelWithPaddingFromInput(in_y, in_x, in_row, in_col, input) * kernel[ky + offset][kx + offset];
                }
            }
            output[y][x] = sum;
        }
    } 
    
}
int main() {
    // Create a sample grayscale image (5x5)
    GrayscaleImage img(5, 5);

    // Initialize the image with some values
    uint8_t values[5][5] = {
        {10, 20, 30, 40, 50},
        {20, 30, 40, 50, 60},
        {30, 40, 50, 60, 70},
        {40, 50, 60, 70, 80},
        {50, 60, 70, 80, 90}
    };


    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            img.setPixel(x, y, values[y][x]);
        }
    }

    // Define a 5x5 kernel (example: edge detection)
    const int8_t kernel[3][3] = {
        { -1,  0,  1 },
        { -2,  0,  2 },
        { -1,  0,  1 }
    };

    // Print the original image
    std::cout << "Original Image:" << std::endl;
    img.print();

    // Apply convolution with zero-padding
    img.applyConvolutionWithPadding(kernel);
    std::cout << "\nConvolution with Zero-Padding:" << std::endl;
    img.print();

    std::vector<std::vector<int> > input2d = 
    {
        {10, 20, 30, 40, 50},
        {20, 30, 40, 50, 60},
        {30, 40, 50, 60, 70},
        {40, 50, 60, 70, 80},
        {50, 60, 70, 80, 90}
    };
    std::vector<std::vector<int>> knl = {
        { -1,  0,  1 },
        { -2,  0,  2 },
        { -1,  0,  1 }
    }; 
    std::vector<std::vector<int> > output;
    convolutionWithStride(input2d, knl, output, 2);

    std::cout << "Convolution Result with Stride of 2:" << std::endl;
    for (const auto& row : output) {
        for (int val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}


