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

char *format_ipv4_port(const struct sockaddr_in *addr)
{
    char *ip = inet_ntoa(addr->sin_addr);
    int port = htons(addr->sin_port);
    char *ip_port = (char *)malloc(strlen(ip) + 6);
    sprintf(ip_port, "%s:%u", ip, port);
    return ip_port;
}