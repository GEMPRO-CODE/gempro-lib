#ifndef _GEMPRO_MONOIDS_HPP_
#define _GEMPRO_MONOIDS_HPP_

#include <algorithm>

namespace gempro {

template<class T>
struct MaxMon {
	T val;
};

template<class T>
MaxMon<T> operator+(MaxMon<T> a, MaxMon<T> b) {
	return MaxMon<T>{std::max(a.val, b.val)};
}

template<class T>
struct MinMon {
	T val;
};

template<class T>
MinMon<T> operator+(MinMon<T> a, MinMon<T> b) {
	return MinMon<T>{std::min(a.val, b.val)};
}

template<class S>
struct AffineMon {
	S a, b;
};

template<class S>
AffineMon<S> operator+(AffineMon<S> f, AffineMon<S> g) {
	return AffineMon<S>{f.a * g.a, f.a * g.b + f.b};
}

template<class S>
S operator*(AffineMon<S> f, S x) {
	return f.a * x + f.b;
}

};

#endif
