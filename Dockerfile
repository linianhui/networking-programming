# https://hub.docker.com/_/gcc
FROM gcc:10.3 AS builder

WORKDIR /build

COPY . /build

RUN make


# https://hub.docker.com/_/debian
FROM debian:10

WORKDIR /app

ENV PATH=$PATH:/app

COPY --from=builder /build/bin .