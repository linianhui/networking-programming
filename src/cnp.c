#include "cnp.h"

void register_signal()
{
    // signal(SIGHUP, signal_handler);
    // signal(SIGINT, signal_handler);
    // signal(SIGQUIT, signal_handler);
    // signal(SIGKILL, signal_handler);
    // signal(SIGTERM, signal_handler);
    // signal(SIGCHLD, signal_handler);
}

void signal_handler(int signo)
{
    printf_flush("\n[signal] %d %s", signo, strsignal(signo));
}

int create_socket()
{
    return socket_e(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

void init_sockaddr(struct sockaddr_in *addr, const char *ipv4_or_hostname, const char *port)
{
    bzero(addr, sizeof(&addr));
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = hostname_to_ip(ipv4_or_hostname);
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
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(addr, ip_port);
        printf_error("SOCKET ERROR : bind %s", ip_port);
        exit(2);
    }
    return result;
}

int listen_e(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_local_ip_port(sockfd, ip_port);
        printf_error("SOCKET ERROR : listen %s", ip_port);
        exit(3);
    }
    return result;
}

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
    int result = connect(sockfd, servaddr, addrlen);
    if (result == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(servaddr, ip_port);
        printf_error("SOCKET ERROR : connect %s", ip_port);
        exit(4);
    }
    return result;
}

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
    int connect_fd = accept(sockfd, cliaddr, addrlen);
    if (connect_fd == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(cliaddr, ip_port);
        printf_error("SOCKET ERROR : accept %s", ip_port);
        exit(5);
    }
    return connect_fd;
}

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t size = send(sockfd, buf, len, flags);
    if (size == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_remote_ip_port(sockfd, ip_port);
        printf_error("SOCKET ERROR : send to %s", ip_port);
        exit(6);
    }
    return size;
}

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t size = recv(sockfd, buf, len, flags);
    if (size == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_remote_ip_port(sockfd, ip_port);
        printf_error("SOCKET ERROR : recv from %s", ip_port);
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

in_addr_t hostname_to_ip(const char *hostname)
{
    struct hostent *hostent = gethostbyname(hostname);
    if (hostent == NULL)
    {
        return -1;
    }

    if (hostent->h_length < 1)
    {
        return -1;
    }

    struct in_addr *addr = (struct in_addr *)(hostent->h_addr_list[0]);
    in_addr_t ip = addr->s_addr;
    return ip;
}

int get_ip_port(const struct sockaddr *addr, char *ip_port)
{
    char ip[IP_STRING_SIZE];
    char port[PORT_STRING_SIZE];
    int result = getnameinfo(addr, 16,
                             ip, sizeof(ip),
                             port, sizeof(port),
                             NI_NUMERICHOST | NI_NUMERICSERV);
    sprintf(ip_port, "%s:%s", ip, port);
    return 0;
}

int get_local_ip_port(int sockfd, char *ip_port)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getsockname_e(sockfd, (sa *)&addr, &len);
    return get_ip_port((sa *)&addr, ip_port);
}

int get_remote_ip_port(int sockfd, char *ip_port)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    getpeername_e(sockfd, (sa *)&addr, &len);
    return get_ip_port((sa *)&addr, ip_port);
}

int get_local_prompt(int sockfd, char *prompt)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_local_ip_port(sockfd, ip_port);
    sprintf(prompt, "[local  %s]>", ip_port);
    return 0;
}

int get_remote_prompt(int sockfd, char *prompt)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_remote_ip_port(sockfd, ip_port);
    sprintf(prompt, "[remote %s]>", ip_port);
    return 0;
}