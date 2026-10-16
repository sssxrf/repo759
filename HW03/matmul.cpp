#include "matmul.h"

// Parallel version of mmul2 from HW02 (loop order i, k, j). Each thread owns
// whole rows of C, so no two threads ever write the same element.
void mmul(const float *A, const float *B, float *C, const std::size_t n) {
#pragma omp parallel for schedule(static)
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            C[i * n + j] = 0.0f;
        }
        for (std::size_t k = 0; k < n; ++k) {
            for (std::size_t j = 0; j < n; ++j) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}
