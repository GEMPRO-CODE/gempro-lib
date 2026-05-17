#include <random>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/zeta.hpp>

GEMPRO_TEST(zeta_manual) {
    UNUSED_CTX(ctx);
    std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8};

    std::vector<int> subset = values;
    gempro::subsetZeta(subset);
    const std::vector<int> expected_subset{1, 3, 4, 10, 6, 14, 16, 36};
    CHECK_EQ(subset, expected_subset);
    gempro::subsetMobius(subset);
    CHECK_EQ(subset, values);

    std::vector<int> superset = values;
    gempro::supersetZeta(superset);
    const std::vector<int> expected_superset{36, 20, 22, 12, 26, 14, 15, 8};
    CHECK_EQ(superset, expected_superset);
    gempro::supersetMobius(superset);
    CHECK_EQ(superset, values);

    std::vector<int> single{42};
    gempro::subsetZeta(single);
    gempro::supersetZeta(single);
    CHECK_EQ(single, std::vector<int>{42});
}

GEMPRO_TEST_RANDOM(zeta_randomized_roundtrip) {
    std::uniform_int_distribution<int> bit_dist(0, 5);
    std::uniform_int_distribution<int> value_dist(-20, 20);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int bits = bit_dist(ctx.rng);
        int n = 1 << bits;
        std::vector<int> a(static_cast<size_t>(n));
        for (int i = 0; i < n; i++)
            a[static_cast<size_t>(i)] = value_dist(ctx.rng);
        auto b = a;
        gempro::subsetZeta(b);
        gempro::subsetMobius(b);
        CHECK_EQ(b, a);
        b = a;
        gempro::supersetZeta(b);
        gempro::supersetMobius(b);
        CHECK_EQ(b, a);
    }
}
