FROM alpine:3.22 AS builder

RUN apk add --no-cache \
g++ \
make \
musl-dev \
curl-dev \
curl-static \
jsoncpp-dev \
jsoncpp-static

WORKDIR /build
COPY src ./src

RUN g++ -std=c++17 -O2 -Wall -static -s \
./src/*.cpp \
-o aegis-agent \
-lcurl -ljsoncpp -lpthread

FROM scratch
COPY --from=builder /build/aegis-agent /aegis-agent
CMD ["/aegis-agent"]