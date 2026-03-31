#!/bin/bash

set -e -u

./build.sh
ctest --test-dir build --output-on-failure
