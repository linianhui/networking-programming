#include "socket_e.h"

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