SHELL:=/bin/bash

RUIN_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	RUIN_DIR=$(RUIN_DIR) ./build/pong --verbose

debug:
	RUIN_DIR=$(RUIN_DIR) ./build/pong --verbose

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
