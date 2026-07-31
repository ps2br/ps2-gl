#!/bin/sh
set -e

cmake -B .build \
    -DCMAKE_TOOLCHAIN_FILE=cmake/ps2dev.cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_INSTALL_PREFIX=$PS2SDK/ports

cp .build/compile_commands.json .

cmake --build .build
cmake --install .build