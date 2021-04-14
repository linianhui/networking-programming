#include "cnp.h"

int create_socket_ipv4_tcp()
{
    return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

struct sockaddr_in create_sockaddr_ipv4_port(const char *ipv4, const char *port)
{
    struct sockaddr_in addr;
    bzero(&addr, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipv4);
    addr.sin_port = htons(atoi(port));
    return addr;
}