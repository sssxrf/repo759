#include <charconv>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <system_error>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " N (nonnegative integer)\n";
        return 1;
    }

    int n = 0;
    const char* end = argv[1] + std::strlen(argv[1]);
    const auto result = std::from_chars(argv[1], end, n);
    if (result.ec != std::errc{} || result.ptr != end || n < 0) {
        std::cerr << "N must be an integer between 0 and 2147483647.\n";
        return 1;
    }

    for (int i = 0; ; ++i) {
        std::printf("%d%c", i, i == n ? '\n' : ' ');
        if (i == n) {
            break;
        }
    }

    for (int i = n; ; --i) {
        std::cout << i << (i == 0 ? '\n' : ' ');
        if (i == 0) {
            break;
        }
    }

    return 0;
}
