@echo off

set BUILD_DIR=build
set CMAKE_GENERATOR="MinGW Makefiles"

mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake -G %CMAKE_GENERATOR% ..

cmake --build . --config Release --parallel 2

cd ..