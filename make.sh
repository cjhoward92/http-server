#!/bin/bash

set -eux

echo "Creating build dir"
mkdir -p build

cd build

echo "Building"
cmake ..
cmake -DCMAKE_BUILD_TYPE=Debug --build .