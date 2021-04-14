// xxx_e 命名方式，为原始function添加错误处理。
#include "log.h"

int socket_e(int domain, int type, int protocol);

int bind_e(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int listen_e(int sockfd, int backlog);

int connect_e(int sockfd, const struct sockaddr *servaddr, socklen_t addrlen);

int accept_e(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);

ssize_t send_e(int sockfd, const void *buf, size_t len, int flags);

ssize_t recv_e(int sockfd, void *buf, size_t len, int flags);

int close_e(int fd);
