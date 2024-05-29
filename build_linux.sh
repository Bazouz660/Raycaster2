#!/bin/bash

# Create build folder if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Change to build folder
cd build

# Run cmake
cmake ..
cmake --build . --config Release --parallel 2

# Go back to root folder
cd ..