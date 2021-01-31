#!/bin/bash

FLAGS="-g -Wall -Wno-missing-braces"
INCLUDES="-I./src"
LIBRARIES="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

# Build examples
mkdir -p build
cc -o build/shaderbox_demo src/shaderbox_demo.c src/shaderbox.c $FLAGS $INCLUDES $LIBRARIES
