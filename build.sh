#!/bin/bash

mkdir build
cmake -B build .

cp build/compile_commands.json .

cd build
make -j$(nproc)
