FROM ubuntu:25.04

ENV DEBIAN_FRONTEND="noninteractive" \
    TZ="Asia/Shanghai" \
    LANG="C.UTF-8"

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends \
	autoconf \
	autoconf-archive \
	automake \
	build-essential \
	ca-certificates \
	clang \
	cmake \
	curl \
	curl \
	git \
	libegl1-mesa-dev \
	libibus-1.0-dev \
	libltdl-dev \
	libssl-dev \
	libtool \
	libwayland-dev \
	libx11-dev \
	libxext-dev \
	libxft-dev \
	libxkbcommon-dev \
	lld \
	ninja-build \
	pkg-config \
	python3 \
	python3-jinja2 \
	tar \
	unzip \
	wget \
	xz-utils \
	zip \
	&& rm -rf /var/lib/apt/lists/*

RUN wget -q https://github.com/Kitware/CMake/releases/download/v4.1.2/cmake-4.1.2-linux-x86_64.tar.gz && \
	mkdir -p /opt/cmake-4.1.2 && \
	tar -xzf cmake-4.1.2-linux-x86_64.tar.gz -C /opt/cmake-4.1.2 --strip-components=1 && \
	ln -sf /opt/cmake-4.1.2/bin/cmake /usr/local/bin/cmake && \
	rm -f cmake-4.1.2-linux-x86_64.tar.gz

COPY vcpkg.json .

COPY tool/run.sh tool/run.sh

RUN ./tool/run.sh

RUN ./tool/vcpkg/vcpkg install --triplet x64-linux

COPY . .

CMD ["run.sh"]
