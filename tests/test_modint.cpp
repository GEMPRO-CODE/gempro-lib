#include <cassert>
#include <sstream>
#include <vector>

#include <gempro/bit-convolution.hpp>
#include <gempro/modint.hpp>
#include <gempro/segtree.hpp>

int main() {
    using gempro::ModInt;

    static_assert(ModInt::mod() == 998244353);
    static_assert(ModInt{-1}.val() == 998244352);
    static_assert((ModInt{998244352} + ModInt{2}).val() == 1);
    static_assert((ModInt{3} * ModInt{4}).val() == 12);

    ModInt a = 998244352;
    ModInt b = 5;

    assert((a + b).val() == 4);
    assert((b - a).val() == 6);
    assert((-b).val() == 998244348);
    assert((ModInt{3} * ModInt{332748118}).val() == 1);
    assert((ModInt{10} / ModInt{2}).val() == 5);

    ModInt x = 998244352;
    assert((x++).val() == 998244352);
    assert(x.val() == 0);
    assert((++x).val() == 1);
    assert((x--).val() == 1);
    assert(x.val() == 0);
    assert((--x).val() == 998244352);

    assert(ModInt{2}.pow(10).val() == 1024);
    assert((ModInt{123456} * ModInt{123456}.inv()).val() == 1);
    assert(ModInt{7} == ModInt{998244360});
    assert(ModInt{7} != ModInt{8});
    assert(ModInt{7} < ModInt{8});
    assert(ModInt{8} > ModInt{7});
    assert(ModInt{7} <= ModInt{998244360});
    assert(ModInt{8} >= ModInt{7});

    std::stringstream stream;
    stream << ModInt{-3};
    assert(stream.str() == "998244350");
    stream.str("-10");
    stream.clear();
    stream >> x;
    assert(x.val() == 998244343);

    std::vector<ModInt> values{1, 2, 3, 4};
    gempro::Segtree<ModInt> st(values);
    assert(st.query(0, 4) == ModInt{10});
    st.set(1, ModInt{-5});
    assert(st.query(0, 4) == ModInt{3});

    std::vector<ModInt> lhs{1, 2, 3, 4};
    std::vector<ModInt> rhs{5, 6, 7, 8};
    const std::vector<ModInt> expected_or{5, 28, 43, 184};
    const std::vector<ModInt> expected_and{103, 52, 73, 32};

    assert(gempro::orConvolution(lhs, rhs) == expected_or);
    assert(gempro::andConvolution(lhs, rhs) == expected_and);

    return 0;
}
