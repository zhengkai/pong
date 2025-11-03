SHELL:=/bin/bash

RUIN_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	# RUIN_DIR=$(RUIN_DIR) ./build/pong -r 2,2
	RUIN_DIR=$(RUIN_DIR) ./build/pong -r 2,2,3,4,2,2

classic:
	RUIN_DIR=$(RUIN_DIR) ./build/pong -W 25 -H 25 -r 2 -c

debug:
	RUIN_DIR=$(RUIN_DIR) CMAKE_BUILD_TYPE=debug ./run.sh

trace:
	RUIN_DIR=$(RUIN_DIR) ./build/pong --verbose=trace

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
	RUIN_DIR=$(RUIN_DIR) ./build/pong --version

tidy:
	find src -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) | xargs clang-tidy -p build

clean:
	@rm -rf build || :
	./run.sh
