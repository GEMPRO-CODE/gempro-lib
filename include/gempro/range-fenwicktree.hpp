#ifndef _GEMPRO_RANGE_FENWICKTREE_HPP_
#define _GEMPRO_RANGE_FENWICKTREE_HPP_

#include <vector>

namespace gempro {

template <typename T> class RangeFenwicktree {
  public:
    RangeFenwicktree(int size) : s_size(size), t(size + 1) {}
    T query(int pos) {
        T acc{};
        for (int i = pos + 1; i > 0; i -= (-i & i)) {
            acc += t[i];
        }
        return acc;
    }

    void range_add(int l, int r, T delta) {
        add(l, delta);
        add(r + 1, -delta);
    }

  private:
    inline void add(int pos, T delta) {
        for (int i = pos + 1; i <= s_size; i += (-i & i)) {
            t[i] += delta;
        }
    }

    const std::size_t s_size;
    std::vector<T> t;
};

}; // namespace gempro

#endif
