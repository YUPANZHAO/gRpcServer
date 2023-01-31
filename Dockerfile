FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN sed -i s/archive.ubuntu.com/mirrors.aliyun.com/g /etc/apt/sources.list \
    && apt update -y \
    && apt install -y vim git build-essential manpages-dev pkg-config cmake zlib1g zlib1g-dev libssl-dev libre2-dev \
    && apt-get install -y autoconf libtool libgflags-dev libgtest-dev libc++-dev net-tools

RUN mkdir /root/libs-build \
    && cd /root/libs-build \
    && git clone https://ghproxy.com/https://github.com/grpc/grpc.git \
    && cd grpc \
    && git checkout -b v1.46.x v1.46.6 \
    && sed -i "s#https#https://ghproxy.com/https#g" .gitmodules \
    && git submodule update --init \
    && cd third_party/protobuf \
    && sed -i "s#https#https://ghproxy.com/https#g" .gitmodules \
    && git submodule update --init --recursive \
    && ./autogen.sh \
    && ./configure \
    && make -j 8 \
    && make install \
    && ldconfig \
    && cd ../.. \
    && mkdir -p cmake/build \
    && cd cmake/build \
    && cmake ../.. \
    && make -j 8 \
    && make install