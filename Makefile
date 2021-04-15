SRC=src
BIN=bin
CFLAG=

all: init socket-client socket-server

init:
	mkdir -p $(BIN)

socket-client:
	gcc $(CFLAG) $(SRC)/cnp.c $(SRC)/socket-client.c -o $(BIN)/socket-client

socket-server:
	gcc $(CFLAG) $(SRC)/cnp.c $(SRC)/socket-server.c -o $(BIN)/socket-server