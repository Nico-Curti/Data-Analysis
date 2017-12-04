rem cd %HOME%\Dropbox\Codes\Library
rmdir /S /Q build
mkdir build
cd build
rem cmake .. -G "Ninja" "-DCMAKE_CXX_COMPILER=C:/Program Files/LLVM/bin/clang-cl.exe" "-DCMAKE_C_COMPILER=C:/Program Files/LLVM/bin/clang-cl.exe" "-DCMAKE_TOOLCHAIN_FILE=%WORKSPACE%\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
cmake .. -G "Ninja" "-DCMAKE_CXX_COMPILER=C:/Program Files (x86)/LLVM/bin/clang-cl.exe" "-DCMAKE_C_COMPILER=C:/Program Files (x86)/LLVM/bin/clang-cl.exe" "-DCMAKE_TOOLCHAIN_FILE=%WORKSPACE%\vcpkg\scripts\buildsystems\vcpkg.cmake" "-DCMAKE_BUILD_TYPE=Release"
cmake --build .
cd ..
