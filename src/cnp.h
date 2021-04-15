// computer networking programming common header file

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#define SERVER_PORT "12345"
#define BUFFER_SIZE 128

typedef struct sockaddr sa;


#define printf_error(...) {                                             \
    int temp = errno;                                                   \
    fprintf(stderr, __VA_ARGS__);                                       \
    fprintf(stderr, " errno=%d errstr=%s\n", temp, strerror(temp));     \
    fflush(stderr);                                                     \
}

#define printf_flush(...) {                                             \
    fprintf(stdout, __VA_ARGS__);                                       \
    fflush(stdout);                                                     \
}

in_addr_t hostname_to_ip(const char *hostname);

char *get_ip_port(const struct sockaddr *addr);

char *get_sock_ip_port(int sockfd);

char *get_peer_ip_port(int sockfd);

int create_socket();

void init_sockaddr(struct sockaddr_in *addr, const char *ipv4_or_hostname, const char *port);

void init_sockaddr_from_args(struct sockaddr_in *addr, int argc, char *argv[], char *default_ipv4_or_hostname);

int socket_e(int domain, int type, int protocol);

int bind_e(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int listen_e(int sockfd, int backlog);

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags);

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags);

int close_e(int fd);

int getsockname_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int getpeername_e(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

char *get_local_prompt(int sockfd);

char *get_remote_prompt(int sockfd);