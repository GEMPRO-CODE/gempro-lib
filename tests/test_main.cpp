#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

#include "framework/test_framework.hpp"

namespace {

struct Options {
    bool list = false;
    bool only_random = false;
    bool no_random = false;
    bool stress = false;
    std::string filter;
    uint64_t seed = 0xC0FFEE1234567890ULL;
    int random_iters = 100;
};

Options parseArgs(int argc, char **argv) {
    Options opts;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--list") {
            opts.list = true;
        } else if (arg == "--only-random") {
            opts.only_random = true;
        } else if (arg == "--no-random") {
            opts.no_random = true;
        } else if (arg == "--stress") {
            opts.stress = true;
        } else if (arg == "--filter") {
            if (i + 1 >= argc)
                throw std::runtime_error("--filter expects value");
            opts.filter = argv[++i];
        } else if (arg == "--seed") {
            if (i + 1 >= argc)
                throw std::runtime_error("--seed expects value");
            opts.seed = std::stoull(argv[++i]);
        } else if (arg == "--random-iters") {
            if (i + 1 >= argc)
                throw std::runtime_error("--random-iters expects value");
            opts.random_iters = std::stoi(argv[++i]);
        } else {
            throw std::runtime_error("unknown arg: " + arg);
        }
    }
    if (opts.stress && opts.random_iters < 2000) {
        opts.random_iters = 2000;
    }
    if (opts.random_iters < 0) {
        throw std::runtime_error("random iterations must be non-negative");
    }
    if (opts.no_random && opts.only_random) {
        throw std::runtime_error("--no-random and --only-random are incompatible");
    }
    return opts;
}

bool matchesFilter(const std::string &name, const std::string &filter) {
    return filter.empty() || name.find(filter) != std::string::npos;
}

}; // namespace

int main(int argc, char **argv) {
    using gempro::test::Context;
    using gempro::test::Registry;

    Options opts;
    try {
        opts = parseArgs(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Argument error: " << e.what() << "\n";
        return 2;
    }

    const auto &all = Registry::instance().all();
    std::vector<gempro::test::TestCase> selected;
    selected.reserve(all.size());
    for (const auto &tc : all) {
        if (!matchesFilter(tc.name, opts.filter))
            continue;
        if (opts.only_random && !tc.randomized)
            continue;
        if (opts.no_random && tc.randomized)
            continue;
        selected.push_back(tc);
    }
    std::sort(selected.begin(), selected.end(),
              [](const auto &a, const auto &b) { return a.name < b.name; });

    if (opts.list) {
        for (const auto &tc : selected) {
            std::cout << tc.name << (tc.randomized ? " [random]" : " [manual]") << "\n";
        }
        return 0;
    }

    int passed = 0;
    int failed = 0;
    for (const auto &tc : selected) {
        Context ctx;
        ctx.seed = opts.seed;
        ctx.random_iterations = opts.random_iters;
        ctx.rng.seed(gempro::test::mixedSeed(opts.seed, tc.name));
        ctx.iteration = -1;
        std::cout << "Running " << tc.name << "...\n";
        try {
            tc.fn(ctx);
            passed++;
        } catch (const std::exception &e) {
            failed++;
            std::cerr << "FAIL " << tc.name << ": " << e.what() << "\n";
            if (ctx.iteration >= 0) {
                std::cerr << "Failure iteration: " << ctx.iteration << "\n";
            }
            std::cerr << "Reproduce: ./tests/.build/tests_runner --seed " << opts.seed
                      << " --filter " << tc.name;
            if (tc.randomized) {
                std::cerr << " --random-iters " << opts.random_iters;
            }
            std::cerr << "\n";
        }
    }

    std::cout << "Summary: passed " << passed << ", failed " << failed << ", total "
              << selected.size() << "\n";
    return failed == 0 ? 0 : 1;
}
