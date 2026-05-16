#ifndef _GEMPRO_CORE_MATRIX_HPP_
#define _GEMPRO_CORE_MATRIX_HPP_

#include <cassert>
#include <cstdint>
#include <vector>

namespace gempro::core {

template <class S> class Matrix {
  public:
    Matrix() : n(0), m(0), data() {}

    Matrix(int rows, int cols) : n(rows), m(cols), data(rows * cols, S{}) {
        assert(rows >= 0);
        assert(cols >= 0);
    }

    Matrix(int rows, int cols, S value) : n(rows), m(cols), data(rows * cols, value) {
        assert(rows >= 0);
        assert(cols >= 0);
    }

    Matrix(const std::vector<std::vector<S>> &v) : n(static_cast<int>(v.size())) {
        m = n == 0 ? 0 : static_cast<int>(v[0].size());
        for (const auto &row : v) {
            assert(static_cast<int>(row.size()) == m);
        }
        data.reserve(static_cast<size_t>(n) * static_cast<size_t>(m));
        for (const auto &row : v) {
            data.insert(data.end(), row.begin(), row.end());
        }
    }

    static Matrix identity(int size) {
        assert(size >= 0);
        Matrix id(size, size);
        for (int i = 0; i < size; i++) {
            id(i, i) = S{1};
        }
        return id;
    }

    int rows() const { return n; }
    int cols() const { return m; }

    S &operator()(int r, int c) {
        assert(0 <= r && r < n);
        assert(0 <= c && c < m);
        return data[static_cast<size_t>(r) * static_cast<size_t>(m) + static_cast<size_t>(c)];
    }

    const S &operator()(int r, int c) const {
        assert(0 <= r && r < n);
        assert(0 <= c && c < m);
        return data[static_cast<size_t>(r) * static_cast<size_t>(m) + static_cast<size_t>(c)];
    }

    Matrix &operator+=(const Matrix &other) {
        assert(n == other.n);
        assert(m == other.m);
        for (size_t i = 0; i < data.size(); i++) {
            data[i] += other.data[i];
        }
        return *this;
    }

    Matrix &operator-=(const Matrix &other) {
        assert(n == other.n);
        assert(m == other.m);
        for (size_t i = 0; i < data.size(); i++) {
            data[i] -= other.data[i];
        }
        return *this;
    }

    Matrix &operator*=(const S &value) {
        for (S &x : data) {
            x *= value;
        }
        return *this;
    }

    Matrix &operator*=(const Matrix &other) {
        *this = *this * other;
        return *this;
    }

    Matrix pow(int64_t exp) const {
        assert(exp >= 0);
        assert(n == m);
        Matrix base = *this;
        Matrix result = Matrix::identity(n);
        while (exp > 0) {
            if (exp & 1) {
                result *= base;
            }
            base *= base;
            exp >>= 1;
        }
        return result;
    }

    friend Matrix operator+(Matrix lhs, const Matrix &rhs) { return lhs += rhs; }
    friend Matrix operator-(Matrix lhs, const Matrix &rhs) { return lhs -= rhs; }

    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs) {
        assert(lhs.m == rhs.n);
        Matrix out(lhs.n, rhs.m, S{});
        for (int i = 0; i < lhs.n; i++) {
            for (int k = 0; k < lhs.m; k++) {
                const S lv = lhs(i, k);
                if (lv == S{}) {
                    continue;
                }
                const size_t out_row = static_cast<size_t>(i) * static_cast<size_t>(out.m);
                const size_t rhs_row = static_cast<size_t>(k) * static_cast<size_t>(rhs.m);
                for (int j = 0; j < rhs.m; j++) {
                    out.data[out_row + static_cast<size_t>(j)] +=
                        lv * rhs.data[rhs_row + static_cast<size_t>(j)];
                }
            }
        }
        return out;
    }

    friend Matrix operator*(Matrix lhs, const S &value) { return lhs *= value; }
    friend Matrix operator*(const S &value, Matrix rhs) { return rhs *= value; }

    friend bool operator==(const Matrix &lhs, const Matrix &rhs) {
        return lhs.n == rhs.n && lhs.m == rhs.m && lhs.data == rhs.data;
    }

    friend bool operator!=(const Matrix &lhs, const Matrix &rhs) { return !(lhs == rhs); }

  private:
    int n, m;
    std::vector<S> data;
};

}; // namespace gempro::core

#endif
