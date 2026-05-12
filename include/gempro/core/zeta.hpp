#ifndef _GEMPRO_CORE_ZETA_HPP_
#define _GEMPRO_CORE_ZETA_HPP_

#include <vector>

namespace gempro::core {

template<class S>
void subsetZeta(std::vector<S> &a) {
	int n = std::size(a);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 1; i < n; i++)
			if (i & k)
				a[i] = a[i ^ k] + a[i];
}

template<class S>
void subsetMobius(std::vector<S> &a) {
	int n = std::size(a);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 1; i < n; i++)
			if (i & k)
				a[i] = a[i] - a[i ^ k];
}

template<class S>
void supersetZeta(std::vector<S> &a) {
	int n = std::size(a);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 1; i < n; i++)
			if (i & k)
				a[i ^ k] = a[i] + a[i ^ k];
}

template<class S>
void supersetMobius(std::vector<S> &a) {
	int n = std::size(a);
	for (int k = 1; k < n; k <<= 1)
		for (int i = 1; i < n; i++)
			if (i & k)
				a[i ^ k] = a[i ^ k] - a[i];
}

};

#endif
