#include <iostream>

// Struct to represent a histogram for a single channel (Red, Green, Blue, or Alpha)
struct Histogram {
    int bucket1 = 0;  // Pixels with values in [0, 127]
    int bucket2 = 0;  // Pixels with values in [128, 255]

    // Method to add a value to the correct bucket
    void addValue(unsigned char value) {
        if (value <= 127) {
            ++bucket1;
        } else {
            ++bucket2;
        }
    }

    // Method to print the histogram
    void print(const std::string& channelName) const {
        std::cout << channelName << " channel: [0-127]: " << bucket1 << " pixels, [128-255]: " << bucket2 << " pixels" << std::endl;
    }
};

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