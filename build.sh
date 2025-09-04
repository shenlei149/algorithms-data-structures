#!/bin/bash

mkdir build
cmake -B build .

cp build/compile_commands.json .

cmake --build build -j$(nproc)
