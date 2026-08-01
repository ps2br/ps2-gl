#!/bin/sh
set -e

rm -rf .build

cmake -B .build \
    -DCMAKE_TOOLCHAIN_FILE=../cmake/ps2dev.cmake \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cp .build/compile_commands.json .

cmake --build .build

rm -rf ps2iso
mkdir -p ps2iso

cat > ps2iso/SYSTEM.CNF <<EOF
BOOT2 = cdrom0:\PS2GLSAMPLE.ELF;1
VER = 1.00
VMODE = NTSC
EOF

cp .build/ps2gl_sample ps2iso/PS2GLSAMPLE.ELF

mkisofs \
    -o PS2GLSAMPLE.ISO \
    -V "PS2GLSAMPLE" \
    -sysid "PLAYSTATION" \
    -iso-level 1 \
    -full-iso9660-filenames \
    -relaxed-filenames \
    -no-pad \
    ps2iso
