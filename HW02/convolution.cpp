#include "convolution.h"

// Value of f[i, j] with the padding rules applied: inside the image -> image
// value; outside in exactly one dimension (edge) -> 1; outside in both
// dimensions (corner) -> 0.
static inline float padded(const float *image, long i, long j, long n) {
    const bool i_in = (i >= 0 && i < n);
    const bool j_in = (j >= 0 && j < n);
    if (i_in && j_in) {
        return image[i * n + j];
    }
    return (i_in || j_in) ? 1.0f : 0.0f;
}

void convolve(const float *image, float *output, std::size_t n, const float *mask, std::size_t m) {
    const long N = static_cast<long>(n);
    const long M = static_cast<long>(m);
    const long half = (M - 1) / 2;

    for (long x = 0; x < N; ++x) {
        for (long y = 0; y < N; ++y) {
            float sum = 0.0f;
            const bool interior = (x - half >= 0 && x + half < N && y - half >= 0 && y + half < N);

            if (interior) {
                // Fast path: the whole mask window lies inside the image.
                const float *window = image + (x - half) * N + (y - half);
                for (long i = 0; i < M; ++i) {
                    for (long j = 0; j < M; ++j) {
                        sum += mask[i * M + j] * window[i * N + j];
                    }
                }
            } else {
                for (long i = 0; i < M; ++i) {
                    for (long j = 0; j < M; ++j) {
                        sum += mask[i * M + j] * padded(image, x + i - half, y + j - half, N);
                    }
                }
            }

            output[x * N + y] = sum;
        }
    }
}
