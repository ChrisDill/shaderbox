#!/bin/bash

FLAGS="-g -Wall -Wno-missing-braces"
INCLUDES="-I./src"
LIBRARIES="-lraylib -lGL -lm -lpthread -ldl -lrt -lX11"

clang -o shaderbox src/shaderbox_demo.c $FLAGS $INCLUDES $LIBRARIES
