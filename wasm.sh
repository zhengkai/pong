#!/bin/bash -ex

cd "$(dirname "$(readlink -f "$0")")" || exit 1

mkdir -p static/tmp
if [ -z "$PONG_DIR" ]; then
	PONG_DIR="$(pwd)/static"
	export PONG_DIR
fi

if [ -f /www/repo/emsdk/emsdk_env.sh ]; then
	source /www/repo/emsdk/emsdk_env.sh
fi

export CC=clang
export CXX=clang++

# cmake -DCMAKE_BUILD_TYPE=debug \
emcmake cmake \
	-B build-wasm \
	-DCMAKE_TOOLCHAIN_FILE="/www/repo/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
	-DCMAKE_PREFIX_PATH="/usr/local/src/vcpkg/installed/wasm32-emscripten" \
	-DCMAKE_BUILD_TYPE=Release

emmake cmake --build build-wasm --config Release "-j$(nproc)"
