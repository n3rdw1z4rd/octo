#!/bin/bash

echo(){
    builtin echo "========================================"
    builtin echo -e "=== build: $1"
    builtin echo "========================================"
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
fi

echo "build: done"
