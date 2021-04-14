#include "cnp.h"

int create_socket_ipv4_tcp()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
