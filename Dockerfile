FROM alpine:3.22 AS builder

RUN apk add --no-cache \
g++ \
make \
musl-dev \
pkgconf \
curl-dev \
curl-static \
openssl-dev \
openssl-libs-static \
zlib-dev \
zlib-static \
brotli-dev \
brotli-static \
zstd-dev \
zstd-static \
nghttp2-dev \
nghttp2-static \
libidn2-dev \
libidn2-static \
libpsl-dev \
libpsl-static \
libunistring-dev \
libunistring-static \
jsoncpp-dev \
jsoncpp-static

WORKDIR /build
COPY src ./src

RUN g++ -std=c++17 -O2 -Wall -static -s \
./src/*.cpp \
-o aegis-agent_x86_64 \
$(pkg-config --libs --static libcurl) \
-ljsoncpp -lpthread

FROM scratch
COPY --from=builder /build/aegis-agent_x86_64 /aegis-agent_x86_64
CMD ["/aegis-agent_x86_64"]