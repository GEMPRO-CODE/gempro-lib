#ifndef _GEMPRO_SEGTREE_HPP_
#define _GEMPRO_SEGTREE_HPP_

#include <vector>

namespace gempro {

template <class T> class Segtree {
  public:
	Segtree() {}
	Segtree(int N) { build(N); }
	Segtree(std::vector<T> &v) { build(v); }

	void build(int N) {
		n = N;
		t.assign(2 * n, T{});
	}

	void build(std::vector<T> &v) {
		build(v.size());
		std::copy(v.begin(), v.end(), t.begin() + n);
		for (int i = n - 1; i > 0; i--) {
			t[i] = t[i << 1] + t[i << 1 | 1];
		}
	}

	T query(int l, int r) {
		T al{}, ar{};
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

	void set(int i, T value) {
		t[i += n] = value;
		for (i >>= 1; i > 0; i >>= 1) {
			t[i] = t[i << 1] + t[i << 1 | 1];
		}
	}

	T get(int i, T value) {
		return t[i += n];
	}

  private:
	std::vector<T> t;
	int n;
};

}; // namespace gempro

#endif
