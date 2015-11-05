#!/usr/bin/bash

srcdir=$(dirname "$(readlink -f $0)")

cmake -DCMAKE_TOOLCHAIN_FILE="$srcdir/sdcc_toolchain.cmake" "$srcdir"
