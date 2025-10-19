#!/bin/bash -e

PWD="$(dirname "$(readlink -f "$0")")" && cd "$PWD" || exit 1

mkdir -p static/tmp
if [ -z "$PONG_PWD" ]; then
	PONG_PWD="$(pwd)/static"
	export PONG_PWD
fi

[ -f "./tool/emsdk/emsdk_env.sh" ] && EMSDK_QUIET=1 source "./tool/emsdk/emsdk_env.sh"

PRELOAD_FILE=(
	"circle.webp"
	"JetBrainsMono-Regular.otf"
)
PRELOAD_ARG=""
for file in "${PRELOAD_FILE[@]}"; do
	FILE="/static/${file}"
	PRELOAD_ARG="${PRELOAD_ARG} --preload-file ${PWD}${FILE}@${FILE}"
done

set -x
emcmake cmake \
	-DCMAKE_CXX_FLAGS="-gsource-map -O0" \
	-B build-wasm \
	-DCMAKE_TOOLCHAIN_FILE="${PWD}/tool/vcpkg/scripts/buildsystems/vcpkg.cmake" \
	-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE="${PWD}/tool/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
	-DVCPKG_TARGET_TRIPLET="wasm32-emscripten" \
	-DCMAKE_EXE_LINKER_FLAGS="$PRELOAD_ARG" \
	-DCMAKE_BUILD_TYPE="Debug"

emmake cmake \
	--build build-wasm --config Release "-j$(nproc)"
