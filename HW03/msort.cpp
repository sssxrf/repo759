#include "msort.h"

#include <algorithm>

namespace {

// Merges the sorted halves a[0, mid) and a[mid, n) using tmp[0, n) as scratch
// space, and leaves the merged result in a[0, n).
void merge(int *a, std::size_t mid, std::size_t n, int *tmp) {
    std::size_t i = 0, j = mid, k = 0;
    while (i < mid && j < n) {
        tmp[k++] = (a[j] < a[i]) ? a[j++] : a[i++];
    }
    while (i < mid) {
        tmp[k++] = a[i++];
    }
    while (j < n) {
        tmp[k++] = a[j++];
    }
    std::copy(tmp, tmp + n, a);
}

// Serial bottom-up merge sort (no recursion), used below the threshold.
void serial_sort(int *a, int *tmp, std::size_t n) {
    for (std::size_t width = 1; width < n; width *= 2) {
        for (std::size_t lo = 0; lo < n - width; lo += 2 * width) {
            const std::size_t hi = std::min(lo + 2 * width, n);
            merge(a + lo, width, hi - lo, tmp + lo);
        }
    }
}

// Sorts a[0, n): the left half is sorted in a new OpenMP task while the current
// thread sorts the right half; the halves are merged once both are done.
void msort_task(int *a, int *tmp, std::size_t n, std::size_t threshold) {
    if (n < threshold || n < 2) {
        serial_sort(a, tmp, n);
        return;
    }

    const std::size_t mid = n / 2;
#pragma omp task
    msort_task(a, tmp, mid, threshold);
    msort_task(a + mid, tmp + mid, n - mid, threshold);
#pragma omp taskwait

    merge(a, mid, n, tmp);
}

}  // namespace

void msort(int *arr, const std::size_t n, const std::size_t threshold) {
    if (n < 2) {
        return;
    }

    int *tmp = new int[n];
#pragma omp parallel
#pragma omp single
    msort_task(arr, tmp, n, threshold);
    delete[] tmp;
}
