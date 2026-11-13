#include <cstdio>

// Each of the 8 threads computes the factorial of one integer a = 1..8 and
// prints "a!=b".
__global__ void factorial() {
    const int a = threadIdx.x + 1;
    int b = 1;
    for (int i = 2; i <= a; ++i) {
        b *= i;
    }
    std::printf("%d!=%d\n", a, b);
}

int main() {
    factorial<<<1, 8>>>();

    // Wait for the kernel (and its printf output) to finish before exiting.
    cudaDeviceSynchronize();
    return 0;
}
