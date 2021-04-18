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
    log_debug("\n[signal] %d %s", signo, strsignal(signo));
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
        log_error("\nSOCKET ERROR : create");
        return -1;
    }
    log_socket_socket(sockfd);
    return sockfd;
}

int bind_e(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    int result = bind(sockfd, addr, addrlen);
    if (result == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(addr, ip_port);
        log_error("\nSOCKET ERROR : bind %s", ip_port);
        return -1;
    }
    log_socket_bind(sockfd, addr);
    return result;
}

int listen_e(int sockfd, int backlog)
{
    int result = listen(sockfd, backlog);
    if (result == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_local_ip_port(sockfd, ip_port);
        log_error("\nSOCKET ERROR : listen %s", ip_port);
        return -1;
    }
    log_socket_listen(sockfd, backlog);
    return result;
}

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen)
{
    int result = connect(sockfd, servaddr, addrlen);
    if (result == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(servaddr, ip_port);
        log_error("\nSOCKET ERROR : connect %s", ip_port);
        return -1;
    }
    log_socket_connect(sockfd);
    return result;
}

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
    int connect_fd = accept(sockfd, cliaddr, addrlen);
    if (connect_fd == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_ip_port(cliaddr, ip_port);
        log_error("\nSOCKET ERROR : accept %s", ip_port);
        return -1;
    }
    log_socket_accept(sockfd, connect_fd);
    return connect_fd;
}

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags)
{
    ssize_t size = send(sockfd, buf, len, flags);
    if (size == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_remote_ip_port(sockfd, ip_port);
        log_error("\nSOCKET ERROR : send to %s", ip_port);
        return -1;
    }
    log_socket_send(sockfd, (char *)buf);
    return size;
}

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags)
{
    ssize_t size = recv(sockfd, buf, len, flags);
    if (size == -1)
    {
        char ip_port[IP_PORT_STRING_SIZE];
        get_remote_ip_port(sockfd, ip_port);
        log_error("\nSOCKET ERROR : recv from %s", ip_port);
        return -1;
    }
    log_socket_recv(sockfd, (char *)buf);
    // 0代表接收到了remote发送的FIN，代表关闭连接。
    if (size == 0)
    {
        log_debug("FIN");
    }
    return size;
}

int close_e(int fd)
{
    int result = close(fd);
    if (result == -1)
    {
        log_error("\nSOCKET ERROR : close fd=%d", fd);
        return -1;
    }
    log_socket_close(fd);
    return result;
}

int getsockname_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int result = getsockname(sockfd, addr, addrlen);
    if (result == -1)
    {
        log_error("SOCKET ERROR : getsockname");
        exit(9);
    }
    return result;
}

int getpeername_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    int result = getpeername(sockfd, addr, addrlen);
    if (result == -1)
    {
        log_error("SOCKET ERROR : getpeername");
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
    sprintf(prompt, "[local  %s] : ", ip_port);
    return 0;
}

int get_remote_prompt(int sockfd, char *prompt)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_remote_ip_port(sockfd, ip_port);
    sprintf(prompt, "[remote %s] : ", ip_port);
    return 0;
}

int socket_create_bind_listen(int argc, char *argv[])
{
    register_signal();

    pid_t pid = getpid();
    log_debug("%s srart pid %d", argv[0], pid);

    int listen_fd = create_socket();

    struct sockaddr_in servaddr;
    init_sockaddr_from_args(&servaddr, argc, argv, "0.0.0.0");

    bind_e(listen_fd, (sa *)&servaddr, sizeof(servaddr));
    listen_e(listen_fd, 10);

    return listen_fd;
}

int socket_create_connect(int argc, char *argv[])
{
    register_signal();

    pid_t pid = getpid();
    log_debug("%s srart pid %d", argv[0], pid);

    int connect_fd = create_socket();

    struct sockaddr_in servaddr;
    init_sockaddr_from_args(&servaddr, argc, argv, "127.0.0.1");

    // 建立连接，阻塞
    connect_e(connect_fd, (sa *)&servaddr, sizeof(servaddr));

    return connect_fd;
}

int socket_revc_and_send(int connect_fd, char *buf)
{
    int recv_size = recv_e(connect_fd, buf, sizeof(buf), 0);
    if (recv_size == 0)
    {
        close_e(connect_fd);
        return 0;
    }

    for (size_t i = 0; i < recv_size; i++)
    {
        buf[i] = toupper(buf[i]);
    }

    send_e(connect_fd, buf, recv_size + 1, 0);

    return recv_size;
}

void log_stdin_prompt()
{
    pid_t pid = getpid();
    log_debug("[STDIN fgets] pid=%d waiting for input > ", pid);
}

void log_socket_socket(int sockfd)
{
    pid_t pid = getpid();

    log_debug("\n[SOCKET socket] pid=%d sockfd=%d", pid, sockfd);
}

void log_socket_bind(int listen_fd, const struct sockaddr *addr)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_ip_port(addr, ip_port);

    pid_t pid = getpid();

    log_debug("\n[SOCKET bind] pid=%d listen_fd=%d bind on %s", pid, listen_fd, ip_port);
}

void log_socket_listen(int listen_fd, int backlog)
{
    pid_t pid = getpid();

    log_debug("\n[SOCKET listen] pid=%d listen_fd=%d backlog=%d waiting for client connect...", pid, listen_fd, backlog);
}

void log_socket_connect(int connect_fd)
{
    char local_ip_port[IP_PORT_STRING_SIZE];
    char remote_ip_port[IP_PORT_STRING_SIZE];

    get_local_ip_port(connect_fd, local_ip_port);
    get_remote_ip_port(connect_fd, remote_ip_port);
    pid_t pid = getpid();

    log_debug("\n[SOCKET connect] %s pid=%d connected to remote=%s on connect_fd=%d\n",
              local_ip_port, pid, remote_ip_port, connect_fd);
}

void log_socket_accept(int listen_fd, int connect_fd)
{
    char local_ip_port[IP_PORT_STRING_SIZE];
    char remote_ip_port[IP_PORT_STRING_SIZE];

    get_local_ip_port(connect_fd, local_ip_port);
    get_remote_ip_port(connect_fd, remote_ip_port);
    pid_t pid = getpid();

    log_debug("\n[SOCKET accept] %s pid=%d listent_fd=%d accept remote=%s connected on connect_fd=%d\n",
              local_ip_port, pid, listen_fd, remote_ip_port, connect_fd);
}

void log_socket_recv(int connect_fd, char *recv_buf)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_remote_ip_port(connect_fd, ip_port);

    pid_t pid = getpid();

    log_debug("[SOCKET recv] pid=%d connect_fd=%d recv from %s : %s", pid, connect_fd, ip_port, recv_buf);
}

void log_socket_send(int connect_fd, char *send_buf)
{
    char ip_port[IP_PORT_STRING_SIZE];
    get_remote_ip_port(connect_fd, ip_port);

    pid_t pid = getpid();

    log_debug("[SOCKET send] pid=%d connect_fd=%d send to %s : %s", pid, connect_fd, ip_port, send_buf);
}

void log_socket_close(int fd)
{
    log_debug("\n[SOCKET close] fd=%d", fd);
}