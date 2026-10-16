#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <string>

#include "matmul.h"

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

    // n x n matrices A and B of random floats in [-1.0, 1.0], row-major.
    const std::size_t size = n * n;
    float *A = new float[size];
    float *B = new float[size];
    float *C = new float[size];

    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < size; ++i) {
        A[i] = dist(gen);
        B[i] = dist(gen);
    }

    // C = A B with t threads.
    omp_set_num_threads(t);
    const auto start = high_resolution_clock::now();
    mmul(A, B, C, n);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    // First element, last element, then the time (ms).
    std::cout << C[0] << "\n";
    std::cout << C[size - 1] << "\n";
    std::cout << elapsed.count() << "\n";

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
