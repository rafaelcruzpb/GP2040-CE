#!/bin/bash
export PICO_SDK_PATH="$PWD/pico-sdk/"
mkdir build
cd build
cmake ..
make