#include "vscale.cuh"

// Each thread does at most one multiplication: b[i] = a[i] * b[i]. Threads in
// the last block whose index is past the end of the arrays do nothing.
__global__ void vscale(const float *a, float *b, unsigned int n) {
    const unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        b[i] = a[i] * b[i];
    }
}
