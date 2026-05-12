#ifndef _GEMPRO_ZETA_HPP_
#define _GEMPRO_ZETA_HPP_

#include <gempro/core/zeta.hpp>

namespace gempro {

template<class S>
inline void subsetZeta(std::vector<S> &a) { core::subsetZeta(a); }

template<class S>
inline void subsetMobius(std::vector<S> &a) { core::subsetMobius(a); }

template<class S>
inline void supersetZeta(std::vector<S> &a) { core::supersetZeta(a); }

template<class S>
inline void supersetMobius(std::vector<S> &a) { core::supersetMobius(a); }

};

#endif
