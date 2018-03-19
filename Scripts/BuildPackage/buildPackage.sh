#!/bin/bash

cd "$(dirname "$0")"

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j6
