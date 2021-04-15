SRC=src
BIN=bin

all: init socket-client socket-server

init:
	mkdir -p $(BIN)

socket-client:
	gcc $(SRC)/cnp.c $(SRC)/socket-client.c -o $(BIN)/socket-client

socket-server:
	gcc $(SRC)/cnp.c $(SRC)/socket-server.c -o $(BIN)/socket-server