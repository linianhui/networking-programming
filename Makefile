SRC=src
BIN=bin

all: socket-client socket-server

socket-client:
	gcc $(SRC)/log.c $(SRC)/socket_e.c $(SRC)/cnp.c $(SRC)/socket-client.c -o $(BIN)/socket-client

socket-server:
	gcc $(SRC)/log.c $(SRC)/socket_e.c $(SRC)/cnp.c $(SRC)/socket-server.c -o $(BIN)/socket-server