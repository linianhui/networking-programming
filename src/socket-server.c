#include "cnp.h"

void echo(int connect_fd, struct sockaddr *cliaddr)
{
    pid_t pid = getpid();
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    char *client_ip_port = format_ipv4_port(cliaddr);

    printf("\nclient %s connected\n", client_ip_port);
    printf("server pid %d connect_fd %d handler\n", pid, connect_fd);

    int recv_size;
    while ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        printf("client %s send\n%s\n", client_ip_port, buf);
        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        if (send_e(connect_fd, buf, recv_size + 1, 0) > 0)
        {
            printf("server pid %d connect_fd %d echo toupper:\n%s\n", pid, connect_fd, buf);
        }
    }
    printf("server pid %d connect_fd %d close connect\n", pid, connect_fd);
    close_e(connect_fd);
}

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf("server srart pid %d\n", pid);

    int listen_fd = create_socket_ipv4_tcp();
    printf("listen_fd %d\n", listen_fd);

    struct sockaddr servaddr = create_sockaddr_ipv4_port_from_args(argc, argv, "0.0.0.0");
    bind_e(listen_fd, &servaddr, sizeof(servaddr));
    listen_e(listen_fd, 10);

    char *server_ip_port = format_ipv4_port(&servaddr);
    printf("listen on %s \nwaiting for client connect...\n", server_ip_port);

    int connect_fd;
    struct sockaddr cliaddr;
    socklen_t addrlen = sizeof(cliaddr);

    while (1)
    {
        connect_fd = accept_e(listen_fd, &cliaddr, &addrlen);
        if (fork() == 0)
        {
            echo(connect_fd, &cliaddr);
            exit(0);
        }
        close_e(connect_fd);
    }
    close_e(listen_fd);
    return 0;
}