#include "cnp.h"

char *get_ip_port(const struct sockaddr *addr)
{
    socklen_t addrlen = sizeof(addr);
    char hostbuf[NI_MAXHOST], ipbuf[NI_MAXSERV];
    int result = getnameinfo(addr, addrlen,
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
        perror("SOCKET create ERROR");
        exit(1);
    }
    return sockfd;
}

int bind_e(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int result = bind(sockfd, addr, addrlen);
    if (result == -1)
    {
        perror("SOCKET bind ERROR");
        exit(2);
    }
    return result;
}

int listen_e(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result == -1)
    {
        perror("SOCKET listen ERROR");
        exit(3);
    }
    return result;
}

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
    int result = connect(sockfd, servaddr, addrlen);
    if (result == -1)
    {
        perror("SOCKET connect ERROR");
        exit(4);
    }
    return result;
}

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
    int connect_fd = accept(sockfd, cliaddr, addrlen);
    if (connect_fd == -1)
    {
        perror("SOCKET accept ERROR");
        exit(5);
    }
    return connect_fd;
}

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t size = send(sockfd, buf, len, flags);
    if (size == -1)
    {
        perror("SOCKET send ERROR");
        exit(6);
    }
    return size;
}

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t size = recv(sockfd, buf, len, flags);
    if (size == -1)
    {
        perror("SOCKET recv ERROR");
        exit(7);
    }
    return size;
}

int close_e(int fd)
{
    int result = close(fd);
    if (result == -1)
    {
        perror("SOCKET close ERROR");
        exit(8);
    }
    return result;
}