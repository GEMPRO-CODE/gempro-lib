#include <cassert>
#include <vector>

#include <gempro/matrix.hpp>
#include <gempro/modint.hpp>

int main() {
    gempro::Matrix<int> z(2, 3);
    assert(z.rows() == 2);
    assert(z.cols() == 3);
    assert(z(0, 0) == 0);
    assert(z(1, 2) == 0);

    gempro::Matrix<int> a({{1, 2, 3}, {4, 5, 6}});
    gempro::Matrix<int> b({{6, 5, 4}, {3, 2, 1}});
    gempro::Matrix<int> c = a + b;
    assert(c == gempro::Matrix<int>({{7, 7, 7}, {7, 7, 7}}));

    c -= gempro::Matrix<int>({{1, 1, 1}, {2, 2, 2}});
    assert(c == gempro::Matrix<int>({{6, 6, 6}, {5, 5, 5}}));

    gempro::Matrix<int> m1({{1, 2, 3}, {4, 5, 6}});
    gempro::Matrix<int> m2({{7, 8}, {9, 10}, {11, 12}});
    gempro::Matrix<int> prod = m1 * m2;
    assert(prod == gempro::Matrix<int>({{58, 64}, {139, 154}}));

    gempro::Matrix<int> s = m1 * 3;
    assert(s == gempro::Matrix<int>({{3, 6, 9}, {12, 15, 18}}));
    assert(2 * m1 == gempro::Matrix<int>({{2, 4, 6}, {8, 10, 12}}));

    gempro::Matrix<int> sq({{1, 2}, {3, 4}});
    gempro::Matrix<int> id = gempro::Matrix<int>::identity(2);
    assert(sq * id == sq);
    assert(id * sq == sq);

    gempro::Matrix<int> fib({{1, 1}, {1, 0}});
    gempro::Matrix<int> fib5 = fib.pow(5);
    assert(fib5 == gempro::Matrix<int>({{8, 5}, {5, 3}}));
    assert(fib.pow(0) == gempro::Matrix<int>::identity(2));

    using gempro::ModInt;
    gempro::Matrix<ModInt> mod_a({{1, 2}, {3, 4}});
    gempro::Matrix<ModInt> mod_b({{5, 6}, {7, 8}});
    gempro::Matrix<ModInt> mod_prod = mod_a * mod_b;
    assert(mod_prod(0, 0) == ModInt{19});
    assert(mod_prod(0, 1) == ModInt{22});
    assert(mod_prod(1, 0) == ModInt{43});
    assert(mod_prod(1, 1) == ModInt{50});
    assert(mod_a.pow(2) == gempro::Matrix<ModInt>({{7, 10}, {15, 22}}));

    return 0;
}
