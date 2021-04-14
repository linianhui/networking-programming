// computer networking programming common header file

#include "socket_e.h"

int create_socket_ipv4_tcp();

struct sockaddr_in create_sockaddr_ipv4_port(const char *ipv4, const char *port);