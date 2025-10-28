#ifndef _GEMPRO_SIEVE_HPP_
#define _GEMPRO_SIEVE_HPP_

#include <vector>

namespace gempro {

class Sieve {
  public:
	Sieve(int N) : spf(N + 1) {
		for (int i = 2; i <= N; i++) {
			if (spf[i] == 0) {
				spf[i] = i;
				primes.push_back(i);
			}
			for (int j : primes) {
				if (i > N / j) {
					break;
				}
				spf[i * j] = j;
				if (j == spf[i]) {
					break;
				}
			}
		}
	}
	const std::vector<int> &getPrimes() { return primes; }
	/**
	 * Smallest prime factor
	 */
	int getSPF(int n) { return spf[n]; }
	std::vector<int> getPrimeDivisors(int n) {
		std::vector<int> divs;
		while (n > 1) {
			if (divs.size() && divs.back() == spf[n]) {
				n /= spf[n];
			} else {
				divs.push_back(spf[n]);
				n /= spf[n];
			}
		}
		return divs;
	}

  private:
	std::vector<int> primes;
	std::vector<int> spf;
};

}; // namespace gempro

#endif
