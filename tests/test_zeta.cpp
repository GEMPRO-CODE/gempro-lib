#include <cassert>
#include <vector>

#include <gempro/zeta.hpp>

int main() {
    std::vector<int> values{1, 2, 3, 4, 5, 6, 7, 8};

    std::vector<int> subset = values;
    gempro::subsetZeta(subset);
    const std::vector<int> expected_subset{1, 3, 4, 10, 6, 14, 16, 36};
    assert(subset == expected_subset);
    gempro::subsetMobius(subset);
    assert(subset == values);

    std::vector<int> superset = values;
    gempro::supersetZeta(superset);
    const std::vector<int> expected_superset{36, 20, 22, 12, 26, 14, 15, 8};
    assert(superset == expected_superset);
    gempro::supersetMobius(superset);
    assert(superset == values);

    std::vector<int> single{42};
    gempro::subsetZeta(single);
    gempro::supersetZeta(single);
    assert(single == std::vector<int>{42});

    return 0;
}
