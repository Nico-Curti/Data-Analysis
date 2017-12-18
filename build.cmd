rem cd %WORKSPACE%\Data-Analysis

rem CC="clang-cl.exe"
rem CXX="clang-cl.exe"
rem set CC="clang.exe"
rem set CXX="clang++.exe"

rmdir /S /Q build
mkdir build
cd build

rem cmake -T"LLVM-vs2015" -G "Visual Studio 15" "-DCMAKE_BUILD_TYPE=Release" ..
cmake -G "Ninja" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build . rem --config Release
cd ..
