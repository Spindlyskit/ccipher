#!/usr/bin/env sh

set -e

mkdir -p build/test
cp -r assets/* build/test
cd build

# Configure
cmake -DCMAKE_BUILD_TYPE=Debug ..
# Build
cmake --build . --config Debug -- -j "$(nproc)"
# Test
cd test
./test_ccipher
