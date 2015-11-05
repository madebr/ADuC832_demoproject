#!/usr/bin/bash

curdir=$(dirname "$(readlink -f $0)")

cd "$curdir"

mkdir -p build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="$curdir/sdcc_toolchain.cmake" "$curdir"
