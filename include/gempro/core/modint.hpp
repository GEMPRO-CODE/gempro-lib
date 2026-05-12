#ifndef _GEMPRO_CORE_MODINT_HPP_
#define _GEMPRO_CORE_MODINT_HPP_

#include <cstdint>
#include <iostream>
#include <type_traits>

namespace gempro::core {

class ModInt {
  public:
    static constexpr int mod() { return MOD; }

    constexpr ModInt() : v(0) {}

    template <class T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    constexpr ModInt(T value) : v(normalize(value)) {}

    static constexpr ModInt raw(uint32_t value) {
        ModInt x;
        x.v = value;
        return x;
    }

    constexpr int val() const { return static_cast<int>(v); }

    constexpr ModInt operator+() const { return *this; }
    constexpr ModInt operator-() const { return v == 0 ? ModInt{} : raw(MOD - v); }

    constexpr ModInt &operator+=(ModInt other) {
        v += other.v;
        if (v >= MOD)
            v -= MOD;
        return *this;
    }

    constexpr ModInt &operator-=(ModInt other) {
        if (v < other.v)
            v += MOD;
        v -= other.v;
        return *this;
    }

    constexpr ModInt &operator*=(ModInt other) {
        v = static_cast<uint32_t>(static_cast<uint64_t>(v) * other.v % MOD);
        return *this;
    }

    constexpr ModInt &operator/=(ModInt other) { return *this *= other.inv(); }

    constexpr ModInt &operator++() {
        if (++v == MOD)
            v = 0;
        return *this;
    }

    constexpr ModInt &operator--() {
        if (v == 0)
            v = MOD;
        --v;
        return *this;
    }

    constexpr ModInt operator++(int) {
        ModInt copy = *this;
        ++*this;
        return copy;
    }

    constexpr ModInt operator--(int) {
        ModInt copy = *this;
        --*this;
        return copy;
    }

    constexpr ModInt pow(int64_t exp) const {
        ModInt base = *this;
        ModInt result = 1;
        while (exp > 0) {
            if (exp & 1)
                result *= base;
            base *= base;
            exp >>= 1;
        }
        return result;
    }

    constexpr ModInt inv() const { return pow(MOD - 2); }

    friend constexpr ModInt operator+(ModInt lhs, ModInt rhs) { return lhs += rhs; }
    friend constexpr ModInt operator-(ModInt lhs, ModInt rhs) { return lhs -= rhs; }
    friend constexpr ModInt operator*(ModInt lhs, ModInt rhs) { return lhs *= rhs; }
    friend constexpr ModInt operator/(ModInt lhs, ModInt rhs) { return lhs /= rhs; }

    friend constexpr bool operator==(ModInt lhs, ModInt rhs) { return lhs.v == rhs.v; }
    friend constexpr bool operator!=(ModInt lhs, ModInt rhs) { return !(lhs == rhs); }
    friend constexpr bool operator<(ModInt lhs, ModInt rhs) { return lhs.v < rhs.v; }
    friend constexpr bool operator>(ModInt lhs, ModInt rhs) { return rhs < lhs; }
    friend constexpr bool operator<=(ModInt lhs, ModInt rhs) { return !(rhs < lhs); }
    friend constexpr bool operator>=(ModInt lhs, ModInt rhs) { return !(lhs < rhs); }

    friend std::ostream &operator<<(std::ostream &out, ModInt value) {
        return out << value.val();
    }

    friend std::istream &operator>>(std::istream &in, ModInt &value) {
        int64_t x;
        in >> x;
        value = ModInt{x};
        return in;
    }

  private:
    static constexpr uint32_t MOD = 998244353;

    template <class T> static constexpr uint32_t normalize(T value) {
        if constexpr (std::is_signed<T>::value) {
            int64_t x = static_cast<int64_t>(value % static_cast<int64_t>(MOD));
            if (x < 0)
                x += MOD;
            return static_cast<uint32_t>(x);
        } else {
            return static_cast<uint32_t>(value % MOD);
        }
    }

    uint32_t v;
};

}; // namespace gempro::core

#endif
