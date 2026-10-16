#include <chrono>
#include <iostream>
#include <random>
#include <ratio>
#include <string>

#include "msort.h"

using std::chrono::duration;
using std::chrono::high_resolution_clock;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " n t ts\n";
        return 1;
    }

    const std::size_t n = std::stoull(argv[1]);
    const int t = std::stoi(argv[2]);
    const std::size_t ts = std::stoull(argv[3]);
    if (n == 0 || t < 1 || t > 20) {
        std::cerr << "n must be positive and t must be in [1, 20].\n";
        return 1;
    }

    // Array of n random ints in [-1000, 1000].
    int *arr = new int[n];
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(-1000, 1000);
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = dist(gen);
    }

    // Sort with t threads.
    omp_set_num_threads(t);
    const auto start = high_resolution_clock::now();
    msort(arr, n, ts);
    const auto end = high_resolution_clock::now();
    const duration<double, std::milli> elapsed = end - start;

    // First element, last element, then the time (ms).
    std::cout << arr[0] << "\n";
    std::cout << arr[n - 1] << "\n";
    std::cout << elapsed.count() << "\n";

    delete[] arr;

    return 0;
}
