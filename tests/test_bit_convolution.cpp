#include <cassert>
#include <vector>

#include <gempro/bit-convolution.hpp>

std::vector<int> bruteOrConvolution(const std::vector<int> &a,
                                    const std::vector<int> &b) {
    std::vector<int> result(a.size());
    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        for (int j = 0; j < static_cast<int>(b.size()); j++) {
            result[i | j] += a[i] * b[j];
        }
    }
    return result;
}

std::vector<int> bruteAndConvolution(const std::vector<int> &a,
                                     const std::vector<int> &b) {
    std::vector<int> result(a.size());
    for (int i = 0; i < static_cast<int>(a.size()); i++) {
        for (int j = 0; j < static_cast<int>(b.size()); j++) {
            result[i & j] += a[i] * b[j];
        }
    }
    return result;
}

int main() {
    const std::vector<int> a{1, 2, 3, 4};
    const std::vector<int> b{5, 6, 7, 8};

    assert(gempro::orConvolution(a, b) == bruteOrConvolution(a, b));
    assert(gempro::andConvolution(a, b) == bruteAndConvolution(a, b));

    const std::vector<int> signed_a{2, -1, 0, 3, 4, -2, 1, 5};
    const std::vector<int> signed_b{-3, 6, 2, -4, 1, 0, 7, -2};

    assert(gempro::orConvolution(signed_a, signed_b) ==
           bruteOrConvolution(signed_a, signed_b));
    assert(gempro::andConvolution(signed_a, signed_b) ==
           bruteAndConvolution(signed_a, signed_b));

    return 0;
}
