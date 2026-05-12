#include <cassert>
#include <utility>
#include <vector>

#include <gempro/sieve.hpp>

int main() {
    gempro::Sieve sieve(50);

    const std::vector<int> expected_primes{2, 3, 5, 7, 11, 13, 17, 19,
                                           23, 29, 31, 37, 41, 43, 47};
    assert(sieve.getPrimes() == expected_primes);

    assert(sieve.getSPF(2) == 2);
    assert(sieve.getSPF(49) == 7);
    assert(sieve.getSPF(50) == 2);

    const std::vector<int> divisors{2, 3, 5};
    assert(sieve.getPrimeDivisors(2 * 3 * 5) == divisors);

    const std::vector<std::pair<int, int>> factorization{{2, 4}, {3, 1}};
    assert(sieve.getPrimeFactorization(2 * 2 * 2 * 2 * 3) == factorization);

    assert(sieve.getPrimeDivisors(1).empty());
    assert(sieve.getPrimeFactorization(1).empty());

    return 0;
}
