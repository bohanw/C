// Assume preprocessLuma is defined somewhere
int8_t preprocessLuma(int8_t pixel);

// Function to apply the 3x3 convolution with preprocessed luma values
void applyConvolutionWithPreprocessing(const int8_t kernel[3][3]) {
    int kernelSize = 3;  // Fixed size for the 3x3 kernel
    int offset = kernelSize / 2;  // Offset for the kernel is 1 (since it's 3x3)

    // Create a buffer for the new image
    uint8_t* newBuffer = new uint8_t[width * height];

    // Create an array (cache) to store the preprocessed luma values
    int8_t* preprocessedLuma = new int8_t[width * height];

    // Precompute and cache the preprocessed luma values for the entire image
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixelIndex = y * width + x;
            preprocessedLuma[pixelIndex] = preprocessLuma(buffer[pixelIndex]);
        }
    }

    // Apply convolution using the precomputed luma values
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int32_t sum = 0;  // Accumulator for the weighted sum

            // Perform the convolution
            for (int ky = -offset; ky <= offset; ++ky) {
                for (int kx = -offset; kx <= offset; ++kx) {
                    int imageX = std::min(std::max(x + kx, 0), width - 1);  // Handle edge cases
                    int imageY = std::min(std::max(y + ky, 0), height - 1);
                    int pixelIndex = imageY * width + imageX;

                    // Use the precomputed luma value and apply the kernel
                    sum += preprocessedLuma[pixelIndex] * kernel[ky + offset][kx + offset];
                }
            }

            // Store the result in the output buffer
            newBuffer[y * width + x] = std::min(255, std::max(0, sum));
        }
    }

    // Copy the result from the temporary buffer to the original buffer
    std::memcpy(buffer, newBuffer, width * height * sizeof(uint8_t));

    // Clean up
    delete[] newBuffer;
    delete[] preprocessedLuma;
}
