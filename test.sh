#!/bin/bash

set -e -u

./build.sh

./build/src/numeric/unittest/BigIntegerTest
./build/src/sort/unittest/SortTest
./build/src/graph/unittest/GraphTest
