# https://hub.docker.com/_/gcc
FROM gcc:10.3

WORKDIR /src

COPY . /src

RUN gcc server.c -o server

RUN gcc client.c -o client

ENTRYPOINT ["./server"]