#!/bin/bash

set -e -u

mkdir -p build
cmake -B build . -DCMAKE_BUILD_TYPE=Debug

cp build/compile_commands.json .

cmake --build build -j$(nproc)
