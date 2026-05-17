#include <algorithm>
#include <random>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/monoids.hpp>
#include <gempro/segtree.hpp>

GEMPRO_TEST(monoids_manual) {
    UNUSED_CTX(ctx);
    gempro::MaxMon<int> max_a{4};
    gempro::MaxMon<int> max_b{9};
    gempro::MaxMon<int> max_c{-2};

    CHECK_EQ((max_a + max_b).val, 9);
    CHECK_EQ((max_c + max_a).val, 4);

    gempro::MinMon<int> min_a{4};
    gempro::MinMon<int> min_b{9};
    gempro::MinMon<int> min_c{-2};

    CHECK_EQ((min_a + min_b).val, 4);
    CHECK_EQ((min_c + min_a).val, -2);

    gempro::AffineMon<int> f{2, 3};
    gempro::AffineMon<int> g{-1, 5};
    gempro::AffineMon<int> composed = f + g;

    CHECK_EQ(f * 7, 17);
    CHECK_EQ(g * 7, -2);
    CHECK_EQ(composed.a, -2);
    CHECK_EQ(composed.b, 13);
    CHECK_EQ(composed * 7, f * (g * 7));

    std::vector<gempro::MaxMon<int>> values{{1}, {7}, {3}, {5}};
    gempro::Segtree<gempro::MaxMon<int>> st(values);

    CHECK_EQ(st.query(0, 4).val, 7);
    CHECK_EQ(st.query(2, 4).val, 5);
    st.set(2, gempro::MaxMon<int>{10});
    CHECK_EQ(st.query(0, 4).val, 10);
    CHECK_EQ(st.get(2).val, 10);
}

GEMPRO_TEST_RANDOM(monoids_randomized_small) {
    std::uniform_int_distribution<int> n_dist(1, 20);
    std::uniform_int_distribution<int> value_dist(0, 50);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int n = n_dist(ctx.rng);
        std::vector<gempro::MaxMon<int>> values(static_cast<size_t>(n));
        std::vector<int> plain(static_cast<size_t>(n));
        for (int i = 0; i < n; i++) {
            int v = value_dist(ctx.rng);
            values[static_cast<size_t>(i)] = {v};
            plain[static_cast<size_t>(i)] = v;
        }
        gempro::Segtree<gempro::MaxMon<int>> st(values);
        std::uniform_int_distribution<int> idx_dist(0, n - 1);
        for (int step = 0; step < 80; step++) {
            int idx = idx_dist(ctx.rng);
            int v = value_dist(ctx.rng);
            st.set(idx, gempro::MaxMon<int>{v});
            plain[static_cast<size_t>(idx)] = v;
            int l = idx_dist(ctx.rng);
            int r = idx_dist(ctx.rng);
            if (l > r)
                std::swap(l, r);
            r++;
            int expect = plain[static_cast<size_t>(l)];
            for (int i = l + 1; i < r; i++) {
                expect = std::max(expect, plain[static_cast<size_t>(i)]);
            }
            CHECK_EQ(st.query(l, r).val, expect);
        }
    }
}
