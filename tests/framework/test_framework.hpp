#ifndef GEMPRO_TEST_FRAMEWORK_HPP
#define GEMPRO_TEST_FRAMEWORK_HPP

#include <cstdint>
#include <functional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace gempro::test {

struct Context {
    uint64_t seed = 0;
    int random_iterations = 0;
    int iteration = -1;
    std::mt19937_64 rng;
};

struct TestCase {
    std::string name;
    bool randomized;
    std::function<void(Context &)> fn;
};

class Registry {
  public:
    static Registry &instance() {
        static Registry reg;
        return reg;
    }

    void add(std::string name, bool randomized, std::function<void(Context &)> fn) {
        tests.push_back(TestCase{std::move(name), randomized, std::move(fn)});
    }

    const std::vector<TestCase> &all() const { return tests; }

  private:
    std::vector<TestCase> tests;
};

class Registration {
  public:
    Registration(const char *name, bool randomized, std::function<void(Context &)> fn) {
        Registry::instance().add(name, randomized, std::move(fn));
    }
};

class Failure : public std::runtime_error {
  public:
    explicit Failure(const std::string &message) : std::runtime_error(message) {}
};

template <class T, class = void> struct IsStreamable : std::false_type {};
template <class T>
struct IsStreamable<T, std::void_t<decltype(std::declval<std::ostream &>()
                                            << std::declval<const T &>())>> : std::true_type {};

template <class T> std::string renderValue(const T &value) {
    if constexpr (IsStreamable<T>::value) {
        std::ostringstream out;
        out << value;
        return out.str();
    } else {
        return "<non-streamable>";
    }
}

inline void failCheck(const char *expr, const char *file, int line, const char *func) {
    std::ostringstream out;
    out << file << ":" << line << " in " << func << " CHECK failed: " << expr;
    throw Failure(out.str());
}

template <class A, class B>
void failEq(const A &a, const B &b, const char *lhs, const char *rhs, const char *file, int line,
            const char *func) {
    std::ostringstream out;
    out << file << ":" << line << " in " << func << " CHECK_EQ failed: " << lhs << " == " << rhs
        << " (lhs=" << renderValue(a) << ", rhs=" << renderValue(b) << ")";
    throw Failure(out.str());
}

inline uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

inline uint64_t hashName(const std::string &name) {
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : name) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ULL;
    }
    return h;
}

inline uint64_t mixedSeed(uint64_t base_seed, const std::string &name) {
    return splitmix64(base_seed ^ hashName(name));
}

}; // namespace gempro::test

#define GEMPRO_TEST(name)                                                                         \
    static void name(gempro::test::Context &);                                                    \
    static gempro::test::Registration name##_registration(#name, false, name);                   \
    static void name(gempro::test::Context &ctx)

#define GEMPRO_TEST_RANDOM(name)                                                                  \
    static void name(gempro::test::Context &);                                                    \
    static gempro::test::Registration name##_registration(#name, true, name);                    \
    static void name(gempro::test::Context &ctx)

#define UNUSED_CTX(ctx) (void)(ctx)

#define CHECK(expr)                                                                               \
    do {                                                                                          \
        if (!(expr))                                                                              \
            gempro::test::failCheck(#expr, __FILE__, __LINE__, __func__);                        \
    } while (0)

#define CHECK_EQ(lhs, rhs)                                                                        \
    do {                                                                                          \
        const auto &gempro_lhs_eval = (lhs);                                                      \
        const auto &gempro_rhs_eval = (rhs);                                                      \
        if (!(gempro_lhs_eval == gempro_rhs_eval))                                                \
            gempro::test::failEq(gempro_lhs_eval, gempro_rhs_eval, #lhs, #rhs, __FILE__,         \
                                 __LINE__, __func__);                                             \
    } while (0)

#endif
