#!/usr/bin/env powershell

mkdir -Force build
cd build

#dynamically linked
cmake .. -T"LLVM-vs2015" -G "Visual Studio 15" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=clang.exe" "-DCMAKE_CXX_COMPILER=clang++.exe" "-DCMAKE_C_COMPILER=clang.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Ninja" "-DCMAKE_MAKE_PROGRAM=ninja.exe" "-DCMAKE_LINKER=clang.exe" "-DCMAKE_CXX_COMPILER=clang-cl.exe" "-DCMAKE_C_COMPILER=clang-cl.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Ninja" "-DCMAKE_CXX_COMPILER=clang-cl.exe" "-DCMAKE_C_COMPILER=clang-cl.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Ninja" "-DCMAKE_CXX_COMPILER=cl.exe" "-DCMAKE_C_COMPILER=cl.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
#cmake .. -G "Visual Studio 15" -T"v141_clang_c2" "-DCMAKE_CXX_COMPILER=cl.exe" "-DCMAKE_C_COMPILER=cl.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"

#statically linked
#cmake .. -G "Ninja" "-DCMAKE_CXX_COMPILER=cl.exe" "-DCMAKE_C_COMPILER=cl.exe" "-DCMAKE_TOOLCHAIN_FILE=$env:WORKSPACE\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DVCPKG_TARGET_TRIPLET=x86-windows-static" "-DCMAKE_BUILD_TYPE=Release"

cmake --build . #--config Release

cd ..
