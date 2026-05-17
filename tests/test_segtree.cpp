#include <algorithm>
#include <random>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/segtree.hpp>

GEMPRO_TEST(segtree_manual) {
    UNUSED_CTX(ctx);
    std::vector<int> values{2, 7, 1, 8, 2, 8, 1, 8};
    gempro::Segtree<int> st(values);

    CHECK_EQ(st.query(0, 0), 0);
    CHECK_EQ(st.query(0, 8), 37);
    CHECK_EQ(st.query(1, 4), 16);
    CHECK_EQ(st.query(3, 5), 10);

    st.set(2, 10);
    CHECK_EQ(st.query(0, 8), 46);
    CHECK_EQ(st.query(1, 4), 25);
    CHECK_EQ(st.query(2, 3), 10);
    CHECK_EQ(st.get(2), 10);

    std::vector<int> uneven_values{2, 7, 1, 8, 2, 8};
    gempro::Segtree<int> uneven(uneven_values);

    CHECK_EQ(uneven.query(0, 6), 28);
    CHECK_EQ(uneven.query(1, 4), 16);
    uneven.set(2, 10);
    CHECK_EQ(uneven.query(0, 6), 37);
    CHECK_EQ(uneven.query(2, 5), 20);

    gempro::Segtree<int> filled(4, 3);
    CHECK_EQ(filled.query(0, 4), 12);
    filled.set(1, -1);
    CHECK_EQ(filled.query(0, 4), 8);
    CHECK_EQ(filled.get(1), -1);
}

GEMPRO_TEST_RANDOM(segtree_randomized_small) {
    std::uniform_int_distribution<int> n_dist(1, 18);
    std::uniform_int_distribution<int> value_dist(-30, 30);
    std::uniform_int_distribution<int> op_dist(0, 1);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        const int n = n_dist(ctx.rng);
        std::vector<int> a(static_cast<size_t>(n));
        for (int i = 0; i < n; i++)
            a[static_cast<size_t>(i)] = value_dist(ctx.rng);
        gempro::Segtree<int> st(a);
        std::uniform_int_distribution<int> idx_dist(0, n - 1);
        for (int step = 0; step < 80; step++) {
            if (op_dist(ctx.rng) == 0) {
                int i = idx_dist(ctx.rng);
                int v = value_dist(ctx.rng);
                st.set(i, v);
                a[static_cast<size_t>(i)] = v;
                CHECK_EQ(st.get(i), v);
            } else {
                int l = idx_dist(ctx.rng);
                int r = idx_dist(ctx.rng);
                if (l > r)
                    std::swap(l, r);
                r++;
                int expect = 0;
                for (int i = l; i < r; i++)
                    expect += a[static_cast<size_t>(i)];
                CHECK_EQ(st.query(l, r), expect);
            }
        }
    }
}
