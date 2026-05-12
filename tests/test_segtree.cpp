#include <cassert>
#include <vector>

#include <gempro/segtree.hpp>

int main() {
    std::vector<int> values{2, 7, 1, 8, 2, 8, 1, 8};
    gempro::Segtree<int> st(values);

    assert(st.query(0, 0) == 0);
    assert(st.query(0, 8) == 37);
    assert(st.query(1, 4) == 16);
    assert(st.query(3, 5) == 10);

    st.set(2, 10);
    assert(st.query(0, 8) == 46);
    assert(st.query(1, 4) == 25);
    assert(st.query(2, 3) == 10);
    assert(st.get(2) == 10);

    std::vector<int> uneven_values{2, 7, 1, 8, 2, 8};
    gempro::Segtree<int> uneven(uneven_values);

    assert(uneven.query(0, 6) == 28);
    assert(uneven.query(1, 4) == 16);
    uneven.set(2, 10);
    assert(uneven.query(0, 6) == 37);
    assert(uneven.query(2, 5) == 20);

    gempro::Segtree<int> filled(4, 3);
    assert(filled.query(0, 4) == 12);
    filled.set(1, -1);
    assert(filled.query(0, 4) == 8);
    assert(filled.get(1) == -1);

    return 0;
}
