#include "cnp.h"

char *hostname_to_ip(const char *hostname)
{
    struct hostent *hostent = gethostbyname(hostname);
    if (hostent == NULL)
    {
        return NULL;
    }

    if (hostent->h_length < 1)
    {
        return NULL;
    }

    struct in_addr *ip = (struct in_addr *)(hostent->h_addr_list[0]);
    return inet_ntoa(*ip);
}

char *get_ip_port(const struct sockaddr *addr)
{
    char hostbuf[NI_MAXHOST], ipbuf[NI_MAXSERV];
    int result = getnameinfo(addr, 16,
                             hostbuf, sizeof(hostbuf),
                             ipbuf, sizeof(ipbuf),
                             NI_NUMERICHOST | NI_NUMERICSERV);
    char *ip_port = (char *)malloc(22);
    sprintf(ip_port, "%s:%s", hostbuf, ipbuf);
    return ip_port;
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

void init_sockaddr(struct sockaddr_in *addr, const char *ipv4_or_hostname, const char *port)
{
    char *ipv4 = hostname_to_ip(ipv4_or_hostname);
    bzero(addr, sizeof(&addr));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ipv4);
    addr->sin_port = htons(atoi(port));
}

void init_sockaddr_from_args(struct sockaddr_in *addr, int argc, char *argv[], char *default_ipv4_or_hostname)
{
    char *port = SERVER_PORT;
    switch (argc)
    {
    case 2:
    {
        default_ipv4_or_hostname = argv[1];
        break;
    }
    case 3:
    {
        default_ipv4_or_hostname = argv[1];
        port = argv[2];
        break;
    }
    default:
        break;
    }

    init_sockaddr(addr, default_ipv4_or_hostname, port);
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

char *get_local_prompt(int sockfd)
{
    char *ip_port = get_sock_ip_port(sockfd);
    char *prompt = (char *)malloc(64);
    sprintf(prompt, "[local  %s]>", ip_port);
    return prompt;
}

char *get_remote_prompt(int sockfd)
{
    char *ip_port = get_peer_ip_port(sockfd);
    char *prompt = (char *)malloc(64);
    sprintf(prompt, "[remote %s]>", ip_port);
    return prompt;
}