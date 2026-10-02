#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <vector>

#include "matmul.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main() {
    const unsigned int n = 1024;
    const std::size_t size = static_cast<std::size_t>(n) * n;

    // Generate A and B (row-major) with random doubles in [-1.0, 1.0].
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    double *A = new double[size];
    double *B = new double[size];
    for (std::size_t i = 0; i < size; ++i) {
        A[i] = dist(gen);
        B[i] = dist(gen);
    }
    // The same matrices stored as std::vector<double> for mmul4.
    const std::vector<double> A_vec(A, A + size);
    const std::vector<double> B_vec(B, B + size);

    double *C = new double[size];

    std::cout << n << "\n";

    // Times one multiplication and prints the time (ms) and the last element of C.
    auto report = [&](auto &&multiply) {
        const auto start = high_resolution_clock::now();
        multiply();
        const auto end = high_resolution_clock::now();
        const duration<double, std::milli> elapsed = end - start;
        std::cout << elapsed.count() << "\n";
        std::cout << C[size - 1] << "\n";
    };

    report([&] { mmul1(A, B, C, n); });
    report([&] { mmul2(A, B, C, n); });
    report([&] { mmul3(A, B, C, n); });
    report([&] { mmul4(A_vec, B_vec, C, n); });

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}
