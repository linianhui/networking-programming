#include "cnp.h"

char *get_ip_port(const struct sockaddr *addr)
{
    char hostbuf[NI_MAXHOST], ipbuf[NI_MAXSERV];
    int result = getnameinfo(addr, 16,
                             hostbuf, sizeof(hostbuf),
                             ipbuf, sizeof(ipbuf),
                             NI_NUMERICHOST | NI_NUMERICSERV);
    if (result == 0)
    {
        char *ip_port = (char *)malloc(strlen(hostbuf) + 1 + strlen(ipbuf));
        sprintf(ip_port, "%s:%s", hostbuf, ipbuf);
        return ip_port;
    }
    return "unknown-ip:unknown-port";
}

char *get_sock_ip_port(int sockfd)
{
    struct sockaddr addr;
    socklen_t len;
    getsockname_e(sockfd, &addr, &len);
    return get_ip_port(&addr);
}

char *get_peer_ip_port(int sockfd)
{
    struct sockaddr addr;
    socklen_t len;
    getpeername_e(sockfd, &addr, &len);
    return get_ip_port(&addr);
}

int create_socket()
{
    return socket_e(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

struct sockaddr create_sockaddr(const char *ipv4, const char *port)
{
    struct sockaddr addr;
    bzero(&addr, sizeof addr);
    struct sockaddr_in *addrin = (struct sockaddr_in *)&addr;
    addrin->sin_family = AF_INET;
    addrin->sin_addr.s_addr = inet_addr(ipv4);
    addrin->sin_port = htons(atoi(port));
    return addr;
}

struct sockaddr create_sockaddr_from_args(int argc, char *argv[], char *default_ip)
{
    char *port = SERVER_PORT;
    switch (argc)
    {
    case 2:
    {
        default_ip = argv[1];
        break;
    }
    case 3:
    {
        default_ip = argv[1];
        port = argv[2];
        break;
    }
    default:
        break;
    }

    return create_sockaddr(default_ip, port);
}

int socket_e(int domain, int type, int protocol)
{
    int sockfd = socket(domain, type, protocol);
    if (sockfd == -1)
    {
        printf_error("SOCKET ERROR : create");
        exit(1);
    }
    return sockfd;
}

int bind_e(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int result = bind(sockfd, addr, addrlen);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : bind %s", get_ip_port(addr));
        exit(2);
    }
    return result;
}

int listen_e(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : listen %s", get_sock_ip_port(sockfd));
        exit(3);
    }
    return result;
}

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
    int result = connect(sockfd, servaddr, addrlen);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : connect %s", get_ip_port(servaddr));
        exit(4);
    }
    return result;
}

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
    int connect_fd = accept(sockfd, cliaddr, addrlen);
    if (connect_fd == -1)
    {
        printf_error("SOCKET ERROR : accept %s", get_ip_port(cliaddr));
        exit(5);
    }
    return connect_fd;
}

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t size = send(sockfd, buf, len, flags);
    if (size == -1)
    {
        printf_error("SOCKET ERROR : send to %s", get_peer_ip_port(sockfd));
        exit(6);
    }
    return size;
}

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t size = recv(sockfd, buf, len, flags);
    if (size == -1)
    {
        printf_error("SOCKET ERROR : recv from %s", get_peer_ip_port(sockfd));
        exit(7);
    }
    return size;
}

int close_e(int fd)
{
    int result = close(fd);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : close");
        exit(8);
    }
    return result;
}

int getsockname_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int result = getsockname(sockfd, addr, addrlen);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : getsockname");
        exit(9);
    }
    return result;
}

int getpeername_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int result = getpeername(sockfd, addr, addrlen);
    if (result == -1)
    {
        printf_error("SOCKET ERROR : getpeername");
        exit(10);
    }
    return result;
}