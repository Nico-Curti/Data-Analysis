#!/usr/bin/env powershell

mkdir -Force build
cd build

#dynamically linked
#cmake .. -T"LLVM-vs2015" -G "Visual Studio 15" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=clang.exe" "-DCMAKE_CXX_COMPILER=clang++.exe" "-DCMAKE_C_COMPILER=clang.exe" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Ninja" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=clang.exe" "-DCMAKE_CXX_COMPILER=clang++.exe" "-DCMAKE_C_COMPILER=clang.exe" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Ninja" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=clang-cl.exe" "-DCMAKE_CXX_COMPILER=clang-cl.exe" "-DCMAKE_C_COMPILER=clang-cl.exe" "-DCMAKE_BUILD_TYPE=Release"
cmake .. -G "Ninja" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=cl.exe" "-DCMAKE_CXX_COMPILER=cl.exe" "-DCMAKE_C_COMPILER=cl.exe" "-DCMAKE_BUILD_TYPE=Release"

cmake --build . #--config Release

cd ..
