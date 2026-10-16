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
        std::cerr << "Usage: " << argv[0] << " n t\n";
        return 1;
    }

    const std::size_t n = std::stoull(argv[1]);
    const int t = std::stoi(argv[2]);
    if (n == 0 || t < 1 || t > 20) {
        std::cerr << "n must be positive and t must be in [1, 20].\n";
        return 1;
    }
    const std::size_t m = 3;

    std::mt19937 gen(std::random_device{}());

    // n x n image of random floats in [-10.0, 10.0], row-major.
    float *image = new float[n * n];
    std::uniform_real_distribution<float> image_dist(-10.0f, 10.0f);
    for (std::size_t i = 0; i < n * n; ++i) {
        image[i] = image_dist(gen);
    }

    // 3 x 3 mask of random floats in [-1.0, 1.0], row-major.
    float *mask = new float[m * m];
    std::uniform_real_distribution<float> mask_dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < m * m; ++i) {
        mask[i] = mask_dist(gen);
    }

    float *output = new float[n * n];

    // Apply the mask with t threads.
    omp_set_num_threads(t);
    const auto start = high_resolution_clock::now();
    convolve(image, output, n, mask, m);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    // First element, last element, then the time (ms).
    std::cout << output[0] << "\n";
    std::cout << output[n * n - 1] << "\n";
    std::cout << elapsed.count() << "\n";

    delete[] image;
    delete[] mask;
    delete[] output;

    return 0;
}
