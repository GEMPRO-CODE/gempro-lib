#ifndef _GEMPRO_FENWICK_HPP_
#define _GEMPRO_FENWICK_HPP_

#include <vector>

namespace gempro {

template <class S> class Fenwick {
  public:
    Fenwick(int N) : t(N), n(N) {}
    void add(int i, S value) {
        for (i++; i <= n; i += i & -i)
            t[i - 1] = t[i - 1] + value;
    }

    int sum(int r) {
        S total{};
        for (; r > 0; r -= r & -r)
            total = t[r - 1] + total;
        return total;
    }

    int sum(int l, int r) { return sum(r) - sum(l); }

  private:
    std::vector<S> t;
    int n;
};

}; // namespace gempro

#endif
