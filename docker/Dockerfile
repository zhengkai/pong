FROM ubuntu:24.04

ENV DEBIAN_FRONTEND="noninteractive" \
    TZ="Asia/Shanghai" \
    LANG="C.UTF-8"

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
	clang \
	lld \
	cmake \
	ninja-build \
	git \
	curl \
	ca-certificates \
	&& rm -rf /var/lib/apt/lists/*

COPY . .

RUN ./tools/run.sh
