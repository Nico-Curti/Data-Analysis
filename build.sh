mkdir -p build
cd build
cmake .. -G "Ninja"
#cmake ..
cmake --build .
cd ..