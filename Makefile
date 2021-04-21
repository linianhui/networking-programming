SRC=src
BIN=bin
CFLAG=
STD=-std=gnu99
OS=$(shell uname -s)
CC=gcc $(STD) $(CFLAG) $(SRC)/bitmap.c $(SRC)/cnp.c

all: init socket-client socket-server select-client select-server poll-client poll-server epoll-client epoll-server

init:
	mkdir -p $(BIN)

socket-client:
	$(CC) $(SRC)/socket-client.c -o $(BIN)/socket-client

socket-server:
	$(CC) $(SRC)/socket-server.c -o $(BIN)/socket-server

select-client:
	$(CC) $(SRC)/select-client.c -o $(BIN)/select-client

select-server:
	$(CC) $(SRC)/select-server.c -o $(BIN)/select-server

poll-client:
	$(CC) $(SRC)/poll-client.c -o $(BIN)/poll-client

poll-server:
	$(CC) $(SRC)/poll-server.c -o $(BIN)/poll-server

epoll-client:
ifeq ($(OS),Linux)
	$(CC) $(SRC)/epoll-client.c -o $(BIN)/epoll-client
else
	@echo 'epoll-client only compile on Linux, current os is $(OS)'
endif

epoll-server:
ifeq ($(OS),Linux)
	$(CC) $(SRC)/epoll-server.c -o $(BIN)/epoll-server
else
	@echo 'epoll-server only compile on Linux, current os is $(OS)'
endif