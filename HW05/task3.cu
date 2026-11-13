#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "vscale.cuh"

// Exits with a message if a CUDA call failed.
static void check(cudaError_t err, const char *what) {
    if (err != cudaSuccess) {
        std::cerr << what << " failed: " << cudaGetErrorString(err) << "\n";
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    // The optional 2nd argument changes the threads per block (default 512);
    // it is used for the 16-threads-per-block comparison in part c).
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n [threads_per_block]\n";
        return 1;
    }
    const unsigned int n = std::stoul(argv[1]);
    const unsigned int threads_per_block = (argc == 3) ? std::stoul(argv[2]) : 512;
    if (n == 0 || threads_per_block == 0 || threads_per_block > 1024) {
        std::cerr << "n must be positive and threads_per_block must be in [1, 1024].\n";
        return 1;
    }
    const std::size_t bytes = static_cast<std::size_t>(n) * sizeof(float);

    // Host arrays: a in [-10.0, 10.0], b in [0.0, 1.0].
    float *a = new float[n];
    float *b = new float[n];
    std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist_a(-10.0f, 10.0f);
    std::uniform_real_distribution<float> dist_b(0.0f, 1.0f);
    for (unsigned int i = 0; i < n; ++i) {
        a[i] = dist_a(gen);
        b[i] = dist_b(gen);
    }

    // Device copies of a and b.
    float *da, *db;
    check(cudaMalloc(&da, bytes), "cudaMalloc(a)");
    check(cudaMalloc(&db, bytes), "cudaMalloc(b)");
    check(cudaMemcpy(da, a, bytes, cudaMemcpyHostToDevice), "copy a to device");
    check(cudaMemcpy(db, b, bytes, cudaMemcpyHostToDevice), "copy b to device");

    // 1D execution configuration: enough blocks to give every element a thread.
    const unsigned int num_blocks = (n + threads_per_block - 1) / threads_per_block;

    // Warm-up launch with n = 0: it changes nothing, but loads the kernel onto
    // the GPU so that this one-time cost is not part of the timed launch.
    vscale<<<1, threads_per_block>>>(da, db, 0);
    check(cudaDeviceSynchronize(), "warm-up launch");

    // Time the kernel with CUDA events.
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start);
    vscale<<<num_blocks, threads_per_block>>>(da, db, n);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    check(cudaGetLastError(), "vscale launch");

    float ms = 0.0f;
    cudaEventElapsedTime(&ms, start, stop);

    // Copy the result back and print the time (ms), then the first and last elements.
    check(cudaMemcpy(b, db, bytes, cudaMemcpyDeviceToHost), "copy b to host");
    std::cout << ms << "\n";
    std::cout << b[0] << "\n";
    std::cout << b[n - 1] << "\n";

    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(da);
    cudaFree(db);
    delete[] a;
    delete[] b;

    return 0;
}
