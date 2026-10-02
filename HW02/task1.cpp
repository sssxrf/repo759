#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <string>

#include "scan.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " n\n";
        return 1;
    }

    const std::size_t n = std::stoull(argv[1]);
    if (n == 0) {
        std::cerr << "n must be a positive integer.\n";
        return 1;
    }

    // i) Create an array of n random floats in [-1.0, 1.0].
    float *arr = new float[n];
    float *output = new float[n];

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = dist(gen);
    }

    // ii) Scan the array.
    const auto start = high_resolution_clock::now();
    scan(arr, output, n);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    // iii) - v) Print the time (ms), then the first and last elements.
    std::cout << elapsed.count() << "\n";
    std::cout << output[0] << "\n";
    std::cout << output[n - 1] << "\n";

    // vi) Deallocate.
    delete[] arr;
    delete[] output;

    return 0;
}
