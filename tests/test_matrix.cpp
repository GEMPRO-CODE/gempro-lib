#include <random>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/matrix.hpp>
#include <gempro/modint.hpp>

static gempro::Matrix<int> bruteMul(const gempro::Matrix<int> &a, const gempro::Matrix<int> &b) {
    gempro::Matrix<int> out(a.rows(), b.cols(), 0);
    for (int i = 0; i < a.rows(); i++) {
        for (int j = 0; j < b.cols(); j++) {
            int total = 0;
            for (int k = 0; k < a.cols(); k++)
                total += a(i, k) * b(k, j);
            out(i, j) = total;
        }
    }
    return out;
}

GEMPRO_TEST(matrix_manual) {
    UNUSED_CTX(ctx);
    gempro::Matrix<int> z(2, 3);
    CHECK_EQ(z.rows(), 2);
    CHECK_EQ(z.cols(), 3);
    CHECK_EQ(z(0, 0), 0);
    CHECK_EQ(z(1, 2), 0);

    gempro::Matrix<int> a({{1, 2, 3}, {4, 5, 6}});
    gempro::Matrix<int> b({{6, 5, 4}, {3, 2, 1}});
    gempro::Matrix<int> c = a + b;
    CHECK_EQ(c, gempro::Matrix<int>({{7, 7, 7}, {7, 7, 7}}));

    c -= gempro::Matrix<int>({{1, 1, 1}, {2, 2, 2}});
    CHECK_EQ(c, gempro::Matrix<int>({{6, 6, 6}, {5, 5, 5}}));

    gempro::Matrix<int> m1({{1, 2, 3}, {4, 5, 6}});
    gempro::Matrix<int> m2({{7, 8}, {9, 10}, {11, 12}});
    gempro::Matrix<int> prod = m1 * m2;
    CHECK_EQ(prod, gempro::Matrix<int>({{58, 64}, {139, 154}}));
    CHECK_EQ(m1.row(0), std::vector<int>({1, 2, 3}));
    CHECK_EQ(m1.row(1), std::vector<int>({4, 5, 6}));
    CHECK_EQ(m1.col(0), std::vector<int>({1, 4}));
    CHECK_EQ(m1.col(2), std::vector<int>({3, 6}));
    gempro::Matrix<int> m1_t = m1.transpose();
    CHECK_EQ(m1_t.rows(), 3);
    CHECK_EQ(m1_t.cols(), 2);
    CHECK_EQ(m1_t, gempro::Matrix<int>({{1, 4}, {2, 5}, {3, 6}}));
    CHECK_EQ(m1_t.transpose(), m1);

    gempro::Matrix<int> s = m1 * 3;
    CHECK_EQ(s, gempro::Matrix<int>({{3, 6, 9}, {12, 15, 18}}));
    CHECK_EQ(2 * m1, gempro::Matrix<int>({{2, 4, 6}, {8, 10, 12}}));

    gempro::Matrix<int> sq({{1, 2}, {3, 4}});
    gempro::Matrix<int> id = gempro::Matrix<int>::identity(2);
    CHECK_EQ(sq * id, sq);
    CHECK_EQ(id * sq, sq);

    gempro::Matrix<int> fib({{1, 1}, {1, 0}});
    gempro::Matrix<int> fib5 = fib.pow(5);
    CHECK_EQ(fib5, gempro::Matrix<int>({{8, 5}, {5, 3}}));
    CHECK_EQ(fib.pow(0), gempro::Matrix<int>::identity(2));

    using gempro::ModInt;
    gempro::Matrix<ModInt> mod_a({{1, 2}, {3, 4}});
    gempro::Matrix<ModInt> mod_b({{5, 6}, {7, 8}});
    gempro::Matrix<ModInt> mod_prod = mod_a * mod_b;
    CHECK_EQ(mod_prod(0, 0), ModInt{19});
    CHECK_EQ(mod_prod(0, 1), ModInt{22});
    CHECK_EQ(mod_prod(1, 0), ModInt{43});
    CHECK_EQ(mod_prod(1, 1), ModInt{50});
    CHECK_EQ(mod_a.pow(2), gempro::Matrix<ModInt>({{7, 10}, {15, 22}}));
    CHECK_EQ(mod_a.transpose(), gempro::Matrix<ModInt>({{1, 3}, {2, 4}}));
    CHECK_EQ((m1 * m2).transpose(), (m2.transpose() * m1.transpose()));
}

GEMPRO_TEST_RANDOM(matrix_randomized_small) {
    std::uniform_int_distribution<int> n_dist(1, 5);
    std::uniform_int_distribution<int> value_dist(-10, 10);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int n = n_dist(ctx.rng);
        int m = n_dist(ctx.rng);
        int k = n_dist(ctx.rng);
        gempro::Matrix<int> a(n, m, 0);
        gempro::Matrix<int> b(m, k, 0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                a(i, j) = value_dist(ctx.rng);
        for (int i = 0; i < m; i++)
            for (int j = 0; j < k; j++)
                b(i, j) = value_dist(ctx.rng);
        CHECK_EQ(a * b, bruteMul(a, b));
        CHECK_EQ(a.transpose().transpose(), a);
        if (n > 0 && m > 0) {
            CHECK_EQ(static_cast<int>(a.row(0).size()), m);
            CHECK_EQ(static_cast<int>(a.col(0).size()), n);
        }
    }
}
