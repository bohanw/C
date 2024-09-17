#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t rgb[3]; // RGB color
} pixel;

typedef struct {
    pixel* pixels;       // Pointer to an array of pixels
    unsigned long width;  // Width of the image
    unsigned long height; // Height of the image
} image;

// Define the 5x5 box blur kernel as a constant array
const float box_blur_kernel[5][5] = {
    {1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25},
    {1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25},
    {1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25},
    {1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25},
    {1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25, 1.0f/25}
};

void blur(const image* input, image* output) {
    // Ensure the output image has the same dimensions as the input
    if (input->width != output->width || input->height != output->height) {
        return; // Dimensions must match
    }

    unsigned long width = input->width;
    unsigned long height = input->height;
    pixel* in_pixels = input->pixels;
    pixel* out_pixels = output->pixels;

    // Define the size of the blur box
    const int box_size = 5;
    const int half_box = box_size / 2;

    for (unsigned long y = 0; y < height; ++y) {
        for (unsigned long x = 0; x < width; ++x) {
            unsigned long r_sum = 0, g_sum = 0, b_sum = 0;
            float weight_sum = 0;

            // Iterate over the 5x5 box
            for (int dy = -half_box; dy <= half_box; ++dy) {
                for (int dx = -half_box; dx <= half_box; ++dx) {
                    int ny = y + dy;
                    int nx = x + dx;

                    // Check if the neighbor pixel is within image bounds
                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        pixel* p = &in_pixels[ny * width + nx];
                        float weight = box_blur_kernel[dy + half_box][dx + half_box];
                        r_sum += p->rgb[0] * weight;
                        g_sum += p->rgb[1] * weight;
                        b_sum += p->rgb[2] * weight;
                        weight_sum += weight;
                    }
                }
            }

            // Calculate the average color values
            pixel* out_pixel = &out_pixels[y * width + x];
            out_pixel->rgb[0] = (uint8_t)(r_sum / weight_sum);
            out_pixel->rgb[1] = (uint8_t)(g_sum / weight_sum);
            out_pixel->rgb[2] = (uint8_t)(b_sum / weight_sum);
        }
    }
}
