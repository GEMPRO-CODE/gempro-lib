#ifndef _GEMPRO_BIT_CONVOLUTION_HPP_
#define _GEMPRO_BIT_CONVOLUTION_HPP_

#include <vector>
#include <gempro/core/zeta.hpp>

namespace gempro {

template<class S>
std::vector<S> orConvolution(std::vector<S> a, std::vector<S> b) {
	int n = std::size(a);
	core::subsetZeta(a);
	core::subsetZeta(b);
	for (int i = 0; i < n; i++) a[i] = a[i] * b[i];
	core::subsetMobius(a);
	return a;
}

template<class S>
std::vector<S> andConvolution(std::vector<S> a, std::vector<S> b) {
	int n = std::size(a);
	core::supersetZeta(a);
	core::supersetZeta(b);
	for (int i = 0; i < n; i++) a[i] = a[i] * b[i];
	core::supersetMobius(a);
	return a;
}

};

#endif
