#!/bin/bash -e

VER="${1:-4.1.2}"

FILE="cmake-${VER}-linux-x86_64.tar.gz"
URL="https://github.com/Kitware/CMake/releases/download/v${VER}/${FILE}"
OPT_DIR="/opt/cmake-${VER}"
TARGET="/usr/local/bin/cmake"

set -x

echo wget -q "$URL"

sudo mkdir -p "$OPT_DIR"

sudo tar -xzf "$FILE" -C "$OPT_DIR" --strip-components=1

sudo ln -sf "${OPT_DIR}/bin/cmake" "$TARGET"

hash -r

cmake --version

rm -f "$FILE"
