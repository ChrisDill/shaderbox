#!/bin/bash

FLAGS="-g -Wall -Wno-missing-braces"
INCLUDES="-I./src"
LIBRARIES="./src/raylib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11"

# Clean build output
find build -type f -executable -delete

# Build examples
mkdir -p build
cc -o build/shaderbox_demo src/shaderbox_demo.c src/shaderbox.c $FLAGS $INCLUDES $LIBRARIES
