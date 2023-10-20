#!/bin/bash

clear

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

echo(){
    builtin echo "*** BUILD: $1"
}

if [ ! -d "build" ]; then
    echo "creating build directory..."
    cmake -B build -S .
else
    echo "cleaning build directory..."
    cmake --build build --target clean
fi

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "building for GNU/Linux..."
    cmake --build build -j$(nproc)
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "building for macOS..."
    cmake --build build -j$(sysctl -n hw.ncpu)
elif [[ "$OSTYPE" == "windows"* ]]; then
    echo "building for Windows..."
fi

echo "done"
echo "to run: bin/test"
