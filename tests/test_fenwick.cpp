#include <cassert>
#include <vector>

#include <gempro/fenwick.hpp>

int main() {
    gempro::Fenwick<int> ft(6);
    std::vector<int> values{3, -1, 4, 1, 5, 9};

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        ft.add(i, values[i]);
    }

    assert(ft.sum(0) == 0);
    assert(ft.sum(1) == 3);
    assert(ft.sum(3) == 6);
    assert(ft.sum(6) == 21);

    assert(ft.sum(0, 6) == 21);
    assert(ft.sum(1, 4) == 4);
    assert(ft.sum(2, 5) == 10);

    ft.add(2, -2);
    assert(ft.sum(3) == 4);
    assert(ft.sum(2, 5) == 8);

    return 0;
}
