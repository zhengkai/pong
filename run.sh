#!/bin/bash -ex

cd "$(dirname "$(readlink -f "$0")")" || exit 1

mkdir -p static/tmp
if [ -z "$PONG_DIR" ]; then
	PONG_DIR="$(pwd)/static"
	export PONG_DIR
fi

export CC=clang
export CXX=clang++

# cmake -DCMAKE_BUILD_TYPE=debug \
cmake \
	-B build \
	-DCMAKE_TOOLCHAIN_FILE="/usr/local/src/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release "-j$(nproc)"

./ldd/run.sh

if [ -e ./build/src/build.h ]; then
	echo cp ./build/src/build.h ./src/
fi

./build/pong --version
