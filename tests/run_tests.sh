#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$root/tests/.build"

mkdir -p "$build_dir"

for test_file in "$root"/tests/test_*.cpp; do
    test_name="$(basename "$test_file" .cpp)"
    binary="$build_dir/$test_name"

    echo "Running $test_name..."
    g++ -std=c++17 -Wall -Wextra -Wpedantic -I"$root/include" "$test_file" -o "$binary"
    "$binary"
done

echo "All tests passed."
