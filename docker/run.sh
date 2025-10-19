#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

sudo docker pull ubuntu:24.04
sudo docker build \
	--progress=plain -t \
	pong \
	-f Dockerfile ..
