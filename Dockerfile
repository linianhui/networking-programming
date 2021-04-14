# https://hub.docker.com/_/gcc
FROM gcc:10.3 AS builder

WORKDIR /build

COPY . /build

RUN gcc src/cnp.c src/socket-server.c -o bin/socket-server

RUN gcc src/cnp.c src/socket-client.c -o bin/socket-client


# https://hub.docker.com/_/debian
FROM debian:10

WORKDIR /app

ENV PATH=$PATH:/app

COPY --from=builder /build/bin .