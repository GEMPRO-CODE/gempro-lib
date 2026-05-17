#include <algorithm>
#include <random>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/fenwick.hpp>

GEMPRO_TEST(fenwick_manual) {
    UNUSED_CTX(ctx);
    gempro::Fenwick<int> ft(6);
    std::vector<int> values{3, -1, 4, 1, 5, 9};

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        ft.add(i, values[i]);
    }

    CHECK_EQ(ft.sum(0), 0);
    CHECK_EQ(ft.sum(1), 3);
    CHECK_EQ(ft.sum(3), 6);
    CHECK_EQ(ft.sum(6), 21);

    CHECK_EQ(ft.sum(0, 6), 21);
    CHECK_EQ(ft.sum(1, 4), 4);
    CHECK_EQ(ft.sum(2, 5), 10);

    ft.add(2, -2);
    CHECK_EQ(ft.sum(3), 4);
    CHECK_EQ(ft.sum(2, 5), 8);
}

GEMPRO_TEST_RANDOM(fenwick_randomized_small) {
    std::uniform_int_distribution<int> n_dist(1, 20);
    std::uniform_int_distribution<int> op_dist(0, 1);
    std::uniform_int_distribution<int> delta_dist(-20, 20);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        const int n = n_dist(ctx.rng);
        gempro::Fenwick<int> ft(n);
        std::vector<int> a(static_cast<size_t>(n), 0);
        std::uniform_int_distribution<int> idx_dist(0, n - 1);
        for (int step = 0; step < 80; step++) {
            if (op_dist(ctx.rng) == 0) {
                int idx = idx_dist(ctx.rng);
                int delta = delta_dist(ctx.rng);
                ft.add(idx, delta);
                a[static_cast<size_t>(idx)] += delta;
            } else {
                int l = idx_dist(ctx.rng);
                int r = idx_dist(ctx.rng);
                if (l > r)
                    std::swap(l, r);
                r++;
                int expect = 0;
                for (int i = l; i < r; i++)
                    expect += a[static_cast<size_t>(i)];
                CHECK_EQ(ft.sum(l, r), expect);
            }
        }
        int total = 0;
        for (int x : a)
            total += x;
        CHECK_EQ(ft.sum(n), total);
    }
}
