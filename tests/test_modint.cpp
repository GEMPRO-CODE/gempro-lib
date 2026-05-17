#include <cstdint>
#include <random>
#include <sstream>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/bit-convolution.hpp>
#include <gempro/modint.hpp>
#include <gempro/segtree.hpp>

static int64_t modNormalize(int64_t x) {
    static constexpr int64_t MOD = 998244353;
    x %= MOD;
    if (x < 0)
        x += MOD;
    return x;
}

GEMPRO_TEST(modint_manual) {
    UNUSED_CTX(ctx);
    using gempro::ModInt;

    static_assert(ModInt::mod() == 998244353);
    static_assert(ModInt{-1}.val() == 998244352);
    static_assert((ModInt{998244352} + ModInt{2}).val() == 1);
    static_assert((ModInt{3} * ModInt{4}).val() == 12);

    ModInt a = 998244352;
    ModInt b = 5;

    CHECK_EQ((a + b).val(), 4);
    CHECK_EQ((b - a).val(), 6);
    CHECK_EQ((-b).val(), 998244348);
    CHECK_EQ((ModInt{3} * ModInt{332748118}).val(), 1);
    CHECK_EQ((ModInt{10} / ModInt{2}).val(), 5);

    ModInt x = 998244352;
    CHECK_EQ((x++).val(), 998244352);
    CHECK_EQ(x.val(), 0);
    CHECK_EQ((++x).val(), 1);
    CHECK_EQ((x--).val(), 1);
    CHECK_EQ(x.val(), 0);
    CHECK_EQ((--x).val(), 998244352);

    CHECK_EQ(ModInt{2}.pow(10).val(), 1024);
    CHECK_EQ((ModInt{123456} * ModInt{123456}.inv()).val(), 1);
    CHECK(ModInt{7} == ModInt{998244360});
    CHECK(ModInt{7} != ModInt{8});
    CHECK(ModInt{7} < ModInt{8});
    CHECK(ModInt{8} > ModInt{7});
    CHECK(ModInt{7} <= ModInt{998244360});
    CHECK(ModInt{8} >= ModInt{7});

    std::stringstream stream;
    stream << ModInt{-3};
    CHECK_EQ(stream.str(), "998244350");
    stream.str("-10");
    stream.clear();
    stream >> x;
    CHECK_EQ(x.val(), 998244343);

    std::vector<ModInt> values{1, 2, 3, 4};
    gempro::Segtree<ModInt> st(values);
    CHECK_EQ(st.query(0, 4), ModInt{10});
    st.set(1, ModInt{-5});
    CHECK_EQ(st.query(0, 4), ModInt{3});

    std::vector<ModInt> lhs{1, 2, 3, 4};
    std::vector<ModInt> rhs{5, 6, 7, 8};
    const std::vector<ModInt> expected_or{5, 28, 43, 184};
    const std::vector<ModInt> expected_and{103, 52, 73, 32};

    CHECK_EQ(gempro::orConvolution(lhs, rhs), expected_or);
    CHECK_EQ(gempro::andConvolution(lhs, rhs), expected_and);
}

GEMPRO_TEST_RANDOM(modint_randomized_small) {
    using gempro::ModInt;
    std::uniform_int_distribution<int64_t> value_dist(-2000000000LL, 2000000000LL);
    std::uniform_int_distribution<int> exp_dist(0, 60);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int64_t a = value_dist(ctx.rng);
        int64_t b = value_dist(ctx.rng);
        ModInt ma{a}, mb{b};
        CHECK_EQ((ma + mb).val(), static_cast<int>(modNormalize(modNormalize(a) + modNormalize(b))));
        CHECK_EQ((ma - mb).val(), static_cast<int>(modNormalize(modNormalize(a) - modNormalize(b))));
        CHECK_EQ((ma * mb).val(), static_cast<int>(modNormalize(modNormalize(a) * modNormalize(b))));

        int e = exp_dist(ctx.rng);
        ModInt p = ma.pow(e);
        int64_t base = modNormalize(a);
        int64_t expect = 1;
        int ee = e;
        while (ee > 0) {
            if (ee & 1)
                expect = modNormalize(expect * base);
            base = modNormalize(base * base);
            ee >>= 1;
        }
        CHECK_EQ(p.val(), static_cast<int>(expect));

        if (mb != ModInt{0}) {
            CHECK_EQ((ma / mb) * mb, ma);
        }
    }
}
