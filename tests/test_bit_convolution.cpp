#include <random>
#include <vector>

#include "framework/test_framework.hpp"
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

GEMPRO_TEST(bit_convolution_manual) {
    UNUSED_CTX(ctx);
    const std::vector<int> a{1, 2, 3, 4};
    const std::vector<int> b{5, 6, 7, 8};

    CHECK_EQ(gempro::orConvolution(a, b), bruteOrConvolution(a, b));
    CHECK_EQ(gempro::andConvolution(a, b), bruteAndConvolution(a, b));

    const std::vector<int> signed_a{2, -1, 0, 3, 4, -2, 1, 5};
    const std::vector<int> signed_b{-3, 6, 2, -4, 1, 0, 7, -2};

    CHECK_EQ(gempro::orConvolution(signed_a, signed_b), bruteOrConvolution(signed_a, signed_b));
    CHECK_EQ(gempro::andConvolution(signed_a, signed_b),
             bruteAndConvolution(signed_a, signed_b));
}

GEMPRO_TEST_RANDOM(bit_convolution_randomized_small) {
    std::uniform_int_distribution<int> bit_dist(0, 5);
    std::uniform_int_distribution<int> value_dist(-10, 10);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int bits = bit_dist(ctx.rng);
        int n = 1 << bits;
        std::vector<int> a(static_cast<size_t>(n));
        std::vector<int> b(static_cast<size_t>(n));
        for (int i = 0; i < n; i++) {
            a[static_cast<size_t>(i)] = value_dist(ctx.rng);
            b[static_cast<size_t>(i)] = value_dist(ctx.rng);
        }
        CHECK_EQ(gempro::orConvolution(a, b), bruteOrConvolution(a, b));
        CHECK_EQ(gempro::andConvolution(a, b), bruteAndConvolution(a, b));
    }
}
