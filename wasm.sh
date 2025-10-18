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

emcmake cmake \
    -DCMAKE_CXX_FLAGS="-gsource-map -O0" \
	-B build-wasm \
	-DCMAKE_TOOLCHAIN_FILE=/usr/local/src/vcpkg/scripts/buildsystems/vcpkg.cmake \
	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/www/repo/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
	-DCMAKE_CROSSCOMPILING_EMULATOR=/www/repo/emsdk/node/22.16.0_64bit/bin/node \
	-DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
	-DCMAKE_EXE_LINKER_FLAGS="--preload-file /www/pong/static/circle.webp@/static/circle.webp --preload-file /www/pong/static/JetBrainsMono-Regular.otf@/static/JetBrainsMono-Regular.otf" \
	-DCMAKE_BUILD_TYPE=Debug

emmake cmake \
	--build build-wasm --config Release "-j$(nproc)"
