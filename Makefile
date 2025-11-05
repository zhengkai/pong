SHELL:=/bin/bash

PONG_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	# PONG_DIR=$(PONG_DIR) ./build/pong -r 2,2
	# PONG_DIR=$(PONG_DIR) ./build/pong -r 2,2,3,4,2,2 --fullscreen
	PONG_DIR=$(PONG_DIR) ./build/pong -r 2,2,3,4,2,2

classic:
	PONG_DIR=$(PONG_DIR) ./build/pong -W 25 -H 26 -r 2 -c

debug:
	PONG_DIR=$(PONG_DIR) CMAKE_BUILD_TYPE=debug ./run.sh

trace:
	PONG_DIR=$(PONG_DIR) ./build/pong --verbose=trace

fullscreen:
	PONG_DIR=$(PONG_DIR) ./build/pong -r 2,2,3,4,2,2 --fullscreen

help:
	./build/pong --help

wasm:
	./wasm.sh

wasm-clean:
	@rm -rf build-wasm || :
	./wasm.sh

ver:
	./build/pong --version

pong:
	./build/pong --pong

small:
	./build/pong --grid-w=20 --grid-h=16

gdb:
	gdb ./build/pong

vc:
	vcpkg install

version:
	PONG_DIR=$(PONG_DIR) ./build/pong --version

tidy:
	find src -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) | xargs clang-tidy -p build

clean:
	@rm -rf build || :
	./run.sh
