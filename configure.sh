#!/usr/bin/env bash
#
# Author: divyekapoor@gmail.com (Divye Kapoor)
#
# Set up gflags dependencies.

set -e

git submodule update --init --recursive

# Sadly, GFlags requires CMake.
# Set up GFlags and build the static and dynamic libraries.
# We will use only the static library though.
cd gflags
mkdir build
cd build
cmake ..

# Actually build the library.
make

