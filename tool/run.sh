#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

if [ ! -d emsdk ]; then
	git clone https://github.com/emscripten-core/emsdk.git
fi
(
	set -x
	cd emsdk || exit 1
	git pull
	./emsdk install latest
	./emsdk activate latest
)

if [ ! -d vcpkg ]; then
	git clone https://github.com/microsoft/vcpkg.git
fi
(
	set -x
	cd vcpkg || exit 1
	git pull
)
