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

VCPKG_DIR="/usr/local/src/vcpkg"
VCPKG_WASM_DIR="${VCPKG_DIR}/installed/wasm32-emscripten"
VCPKG_SHARE_DIR="${VCPKG_WASM_DIR}/share"

emcmake cmake \
    -DCMAKE_CXX_FLAGS="-gsource-map -O0" \
	-B build-wasm \
	-DCMAKE_TOOLCHAIN_FILE=/usr/local/src/vcpkg/scripts/buildsystems/vcpkg.cmake \
	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE=/www/repo/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
	-DCMAKE_CROSSCOMPILING_EMULATOR=/www/repo/emsdk/node/22.16.0_64bit/bin/node \
	-DVCPKG_TARGET_TRIPLET=wasm32-emscripten \
	-DFreetype_DIR="${VCPKG_SHARE_DIR}/freetype" \
	-Dfmt_DIR="${VCPKG_SHARE_DIR}/fmt" \
	-Dspdlog_DIR="${VCPKG_SHARE_DIR}/spdlog" \
	-Dbox2d_DIR="${VCPKG_SHARE_DIR}/box2d" \
    -DSDL3_DIR="${VCPKG_SHARE_DIR}/sdl3" \
    -DSDL3_ttf_DIR="${VCPKG_SHARE_DIR}/sdl3_ttf" \
    -DSDL3_image_DIR="${VCPKG_SHARE_DIR}/SDL3_image" \
	-DCMAKE_EXE_LINKER_FLAGS="--preload-file /www/pong/static/circle.webp@/static/circle.webp --preload-file /www/pong/static/JetBrainsMono-Regular.otf@/static/JetBrainsMono-Regular.otf" \
	-DCMAKE_BUILD_TYPE=Debug
	# -DCMAKE_BUILD_TYPE=Release

emmake cmake \
	--build build-wasm --config Release "-j$(nproc)"
