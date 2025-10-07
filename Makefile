SHELL:=/bin/bash

RUIN_DIR := $(CURDIR)/static

default:
	./run.sh

run:
	RUIN_DIR=$(RUIN_DIR) ./build/pong

debug:
	RUIN_DIR=$(RUIN_DIR) ./build/pong --verbose

trace:
	RUIN_DIR=$(RUIN_DIR) ./build/pong --verbose=trace

ver:
	./build/pong --version

pong:
	./build/pong --pong

gdb:
	gdb ./build/pong

vc:
	vcpkg install

clean:
	@rm -rf build || :
	./run.sh
