#!/bin/bash

clear

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

TARGET=Release
BUILD_DIR=build/release

echo(){
    builtin echo "*** $TARGET BUILD: $1"
}

if [ ! -d "$BUILD_DIR" ]; then
    echo "creating build directory..."
    cmake -DCMAKE_BUILD_TYPE=$TARGET -B $BUILD_DIR -S .
else
    echo "cleaning build directory..."
    cmake --build $BUILD_DIR --target clean
fi

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "building for GNU/Linux..."
    cmake --build $BUILD_DIR -j$(nproc)
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "building for macOS..."
    cmake --build $BUILD_DIR -j$(sysctl -n hw.ncpu)
elif [[ "$OSTYPE" == "windows"* ]]; then
    echo "building for Windows..."
fi

echo "done"
echo "to run: bin/test"
