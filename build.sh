#!/usr/bin/env sh

set -e

mkdir -p build/test
cp -r assets/* build/test
cd build

# Configure
cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..
cd test
make
./ccipher-test
