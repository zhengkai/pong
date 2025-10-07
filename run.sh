#!/bin/bash -ex

cd "$(dirname "$(readlink -f "$0")")" || exit 1

mkdir -p static/tmp
if [ -z "$RUIN_DIR" ]; then
	RUIN_DIR="$(pwd)/static"
	export RUIN_DIR
fi

cmake -DCMAKE_BUILD_TYPE=debug \
	-B build \
	-DCMAKE_TOOLCHAIN_FILE="/usr/local/src/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DCMAKE_BUILD_TYPE=Release

cmake --build build --config Release "-j$(nproc)"

./ldd/run.sh

if [ -e ./build/src/build.h ]; then
	echo cp ./build/src/build.h ./src/
fi

./build/pong --version
