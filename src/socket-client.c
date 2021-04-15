#include "cnp.h"

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf_flush("client srart pid %d\n", pid);

    int connect_fd = create_socket();
    printf_flush("connect_fd %d\n", connect_fd);

    struct sockaddr servaddr = create_sockaddr_from_args(argc, argv, "127.0.0.1");

    connect_e(connect_fd, &servaddr, sizeof(servaddr));

    char *server_ip_port = get_ip_port(&servaddr);
    printf_flush("connected to server %s\n", server_ip_port);

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    while (1)
    {
        bzero(recv_buf, BUFFER_SIZE);
        bzero(send_buf, BUFFER_SIZE);
        fgets(send_buf, BUFFER_SIZE, stdin);
        send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
        recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
        printf_flush("server echo:\n%s\n", recv_buf);
    }
    close_e(connect_fd);
}