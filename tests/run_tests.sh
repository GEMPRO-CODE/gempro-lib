#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build_dir="$root/tests/.build"
runner="$build_dir/tests_runner"

mkdir -p "$build_dir"

echo "Building tests runner..."
g++ -std=c++17 -Wall -Wextra -Wpedantic -I"$root/include" \
    "$root"/tests/test_*.cpp -o "$runner"

echo "Running tests..."
"$runner"
