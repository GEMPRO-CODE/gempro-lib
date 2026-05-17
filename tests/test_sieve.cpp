#include <random>
#include <utility>
#include <vector>

#include "framework/test_framework.hpp"
#include <gempro/sieve.hpp>

static std::vector<int> brutePrimeDivisors(int n) {
    std::vector<int> divs;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            divs.push_back(p);
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        divs.push_back(n);
    return divs;
}

static std::vector<std::pair<int, int>> brutePrimeFactorization(int n) {
    std::vector<std::pair<int, int>> fac;
    for (int p = 2; p * p <= n; p++) {
        if (n % p == 0) {
            int cnt = 0;
            while (n % p == 0) {
                n /= p;
                cnt++;
            }
            fac.push_back({p, cnt});
        }
    }
    if (n > 1)
        fac.push_back({n, 1});
    return fac;
}

GEMPRO_TEST(sieve_manual) {
    UNUSED_CTX(ctx);
    gempro::Sieve sieve(50);

    const std::vector<int> expected_primes{2, 3, 5, 7, 11, 13, 17, 19,
                                           23, 29, 31, 37, 41, 43, 47};
    CHECK_EQ(sieve.getPrimes(), expected_primes);

    CHECK_EQ(sieve.getSPF(2), 2);
    CHECK_EQ(sieve.getSPF(49), 7);
    CHECK_EQ(sieve.getSPF(50), 2);

    const std::vector<int> divisors{2, 3, 5};
    CHECK_EQ(sieve.getPrimeDivisors(2 * 3 * 5), divisors);

    const std::vector<std::pair<int, int>> factorization{{2, 4}, {3, 1}};
    CHECK_EQ(sieve.getPrimeFactorization(2 * 2 * 2 * 2 * 3), factorization);

    CHECK(sieve.getPrimeDivisors(1).empty());
    CHECK(sieve.getPrimeFactorization(1).empty());
}

GEMPRO_TEST_RANDOM(sieve_randomized_small) {
    std::uniform_int_distribution<int> n_dist(2, 200);
    for (int it = 0; it < ctx.random_iterations; it++) {
        ctx.iteration = it;
        int n = n_dist(ctx.rng);
        gempro::Sieve sieve(n);
        std::uniform_int_distribution<int> q_dist(1, n);
        for (int q = 0; q < 30; q++) {
            int x = q_dist(ctx.rng);
            auto divs = sieve.getPrimeDivisors(x);
            auto fac = sieve.getPrimeFactorization(x);
            CHECK_EQ(divs, brutePrimeDivisors(x));
            CHECK_EQ(fac, brutePrimeFactorization(x));
            if (x > 1) {
                CHECK(x % sieve.getSPF(x) == 0);
            }
        }
    }
}
