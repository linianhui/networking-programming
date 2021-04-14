// computer networking programming common header file

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define SERVER_PORT "12345"
#define BUFFER_SIZE 128

int create_socket_ipv4_tcp();

struct sockaddr_in create_sockaddr_ipv4_port(const char *ipv4, const char *port);

char* format_ipv4_port(const struct sockaddr_in *addr);
