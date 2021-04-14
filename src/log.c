#include "log.h"

char *format_ipv4_port(const struct sockaddr_in *addr)
{
    char *ip = inet_ntoa(addr->sin_addr);
    int port = htons(addr->sin_port);
    char *ip_port = (char *)malloc(strlen(ip) + 7);
    sprintf(ip_port, "%s:%u", ip, port);
    return ip_port;
}