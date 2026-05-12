#ifndef _GEMPRO_CORE_SEGTREE_HPP_
#define _GEMPRO_CORE_SEGTREE_HPP_

#include <vector>

namespace gempro::core {

template <class S> class Segtree {
  public:
    Segtree() {}

    Segtree(int N) : t(2 * N, S{}), n(N) {
        for (int i = n - 1; i > 0; i--)
            t[i] = t[i << 1] + t[i << 1 | 1];
    }

    Segtree(int N, S value) : t(2 * N, value), n(N) {
        for (int i = n - 1; i > 0; i--)
            t[i] = t[i << 1] + t[i << 1 | 1];
    }

    Segtree(std::vector<S> &v) : t(2 * std::size(v), S{}), n(std::size(v)) {
        std::copy(std::begin(v), std::end(v), std::begin(t) + n);
        for (int i = n - 1; i > 0; i--)
            t[i] = t[i << 1] + t[i << 1 | 1];
    }

    S query(int l, int r) {
        S al{}, ar{};
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) {
                al = al + t[l++];
            }
            if (r & 1) {
                ar = t[--r] + ar;
            }
        }
        return al + ar;
    }

    void set(int i, S value) {
        t[i += n] = value;
        while (i >>= 1) {
            t[i] = t[i << 1] + t[i << 1 | 1];
        }
    }

    S get(int i) { return t[i + n]; }

  private:
    std::vector<S> t;
    int n;
};

}; // namespace gempro

#endif
