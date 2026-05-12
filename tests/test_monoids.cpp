#include <cassert>
#include <vector>

#include <gempro/monoids.hpp>
#include <gempro/segtree.hpp>

int main() {
    gempro::MaxMon<int> max_a{4};
    gempro::MaxMon<int> max_b{9};
    gempro::MaxMon<int> max_c{-2};

    assert((max_a + max_b).val == 9);
    assert((max_c + max_a).val == 4);

    gempro::MinMon<int> min_a{4};
    gempro::MinMon<int> min_b{9};
    gempro::MinMon<int> min_c{-2};

    assert((min_a + min_b).val == 4);
    assert((min_c + min_a).val == -2);

    gempro::AffineMon<int> f{2, 3};
    gempro::AffineMon<int> g{-1, 5};
    gempro::AffineMon<int> composed = f + g;

    assert(f * 7 == 17);
    assert(g * 7 == -2);
    assert(composed.a == -2);
    assert(composed.b == 13);
    assert(composed * 7 == f * (g * 7));

    std::vector<gempro::MaxMon<int>> values{{1}, {7}, {3}, {5}};
    gempro::Segtree<gempro::MaxMon<int>> st(values);

    assert(st.query(0, 4).val == 7);
    assert(st.query(2, 4).val == 5);
    st.set(2, gempro::MaxMon<int>{10});
    assert(st.query(0, 4).val == 10);
    assert(st.get(2).val == 10);

    return 0;
}
