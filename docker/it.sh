#!/bin/bash

cd "$(dirname "$(readlink -f "$0")")" || exit 1

DIR="$(dirname "$(pwd)")"

mkdir -p "${DIR}/build"

DOCKER_IMAGE="pong"

set -x
sudo docker run \
	-it \
	--name "pong-cicd" \
	--mount "type=bind,source=${DIR}/build,target=/app/build" \
	--rm \
	"$DOCKER_IMAGE" \
	bash
