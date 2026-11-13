#include <cstdio>
#include <random>

// Each thread writes a * x + y into its own entry of dA, where x is the
// thread's index within its block and y is the block's index.
__global__ void axpy(int *dA, int a) {
    const int x = threadIdx.x;
    const int y = blockIdx.x;
    dA[blockIdx.x * blockDim.x + threadIdx.x] = a * x + y;
}

int main() {
    const int num_blocks = 2;
    const int threads_per_block = 8;
    const int size = num_blocks * threads_per_block;

    // Random integer a in [1, 100].
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);
    const int a = dist(gen);

    int *dA;
    cudaMalloc(&dA, size * sizeof(int));

    axpy<<<num_blocks, threads_per_block>>>(dA, a);

    // cudaMemcpy waits for the kernel to finish before copying.
    int hA[size];
    cudaMemcpy(hA, dA, size * sizeof(int), cudaMemcpyDeviceToHost);

    for (int i = 0; i < size; ++i) {
        std::printf("%d%c", hA[i], (i == size - 1) ? '\n' : ' ');
    }

    cudaFree(dA);
    return 0;
}
