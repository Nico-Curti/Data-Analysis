#!/bin/bash

#export CC=gcc
#export CXX=g++

rm -rf build
mkdir -p build
cd build

cmake -G "Ninja" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build .
cd ..
