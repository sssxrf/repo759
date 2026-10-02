#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <string>

#include "convolution.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n m\n";
        return 1;
    }

    const std::size_t n = std::stoull(argv[1]);
    const std::size_t m = std::stoull(argv[2]);
    if (n == 0 || m == 0 || m % 2 == 0) {
        std::cerr << "n must be positive and m must be a positive odd integer.\n";
        return 1;
    }

    std::mt19937 gen(std::random_device{}());

    // i) n x n image of random floats in [-10.0, 10.0], row-major.
    float *image = new float[n * n];
    std::uniform_real_distribution<float> image_dist(-10.0f, 10.0f);
    for (std::size_t i = 0; i < n * n; ++i) {
        image[i] = image_dist(gen);
    }

    // ii) m x m mask of random floats in [-1.0, 1.0], row-major.
    float *mask = new float[m * m];
    std::uniform_real_distribution<float> mask_dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < m * m; ++i) {
        mask[i] = mask_dist(gen);
    }

    float *output = new float[n * n];

    // iii) Apply the mask to the image.
    const auto start = high_resolution_clock::now();
    convolve(image, output, n, mask, m);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    // iv) - vi) Print the time (ms), then the first and last elements.
    std::cout << elapsed.count() << "\n";
    std::cout << output[0] << "\n";
    std::cout << output[n * n - 1] << "\n";

    // vii) Deallocate.
    delete[] image;
    delete[] mask;
    delete[] output;

    return 0;
}
