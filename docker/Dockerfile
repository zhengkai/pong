FROM ubuntu:24.04

ENV DEBIAN_FRONTEND="noninteractive" \
    TZ="Asia/Shanghai" \
    LANG="C.UTF-8"

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
	autoconf-archive\
	autoconf\
	automake\
	build-essential\
	ca-certificates\
	clang \
	cmake \
	curl \
	curl\
	git \
	libssl-dev\
	libtool\
	lld \
	ninja-build \
	pkg-config\
	python3\
	tar\
	unzip\
	wget\
	xz-utils\
	zip\
	&& rm -rf /var/lib/apt/lists/*

RUN wget -q https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2.tar.gz && \
    tar -xzf cmake-4.1.2.tar.gz && \
    cd cmake-4.1.2 && \
    ./bootstrap && \
    make "-j16" && \
    make install && \
	cd .. && \
	rm -rf cmake-4.1.2

COPY tool/run.sh tool/run.sh

RUN ./tool/run.sh


COPY . .

CMD ["run.sh"]
