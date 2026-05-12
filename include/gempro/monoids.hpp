#ifndef _GEMPRO_MONOIDS_HPP_
#define _GEMPRO_MONOIDS_HPP_

namespace gempro {

template<class T>
struct MaxMon {
	T val;
};

template<class T>
MaxMon<T> operator+(MaxMon<T> a, MaxMon<T> b) { return MaxMon{max(a.val, b.val)}; }

template<class T>
struct MinMon {
	T val;
};

template<class T>
MinMon<T> operator+(MinMon<T> a, MinMon<T> b) { return MinMon{min(a.val, b.val)}; }

};

#endif
