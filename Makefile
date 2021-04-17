SRC=src
BIN=bin
CFLAG=

all: init socket-client socket-server select-client

init:
	mkdir -p $(BIN)

socket-client:
	gcc $(CFLAG) $(SRC)/bitmap.c $(SRC)/cnp.c $(SRC)/socket-client.c -o $(BIN)/socket-client

socket-server:
	gcc $(CFLAG) $(SRC)/bitmap.c $(SRC)/cnp.c $(SRC)/socket-server.c -o $(BIN)/socket-server

select-client:
	gcc $(CFLAG) $(SRC)/bitmap.c $(SRC)/cnp.c $(SRC)/select-client.c -o $(BIN)/select-client
