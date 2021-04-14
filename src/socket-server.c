#include "cnp.h"

void echo(int connect_fd, struct sockaddr_in *client_address)
{
    pid_t pid = getpid();
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    char *client_ip_port = format_ipv4_port(client_address);

    printf("\nclient %s connected\n", client_ip_port);
    printf("server pid %d connect_fd %d handler\n", pid, connect_fd);

    int read_size;
    while ((read_size = recv_e(connect_fd, buffer, sizeof(buffer), 0)) != 0)
    {
        printf("client %s send\n%s\n", client_ip_port, buffer);
        for (size_t i = 0; i < read_size; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        if (send_e(connect_fd, buffer, read_size + 1, 0) > 0)
        {
            printf("server pid %d connect_fd %d echo toupper:\n%s\n", pid, connect_fd, buffer);
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

    struct sockaddr_in server_address = create_sockaddr_ipv4_port_from_args(argc, argv, "0.0.0.0");
    bind_e(listen_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    listen_e(listen_fd, 10);

    char *server_ip_port = format_ipv4_port(&server_address);
    printf("listen on %s \nwaiting for client connect...\n", server_ip_port);

    int connect_fd;
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (1)
    {
        connect_fd = accept_e(listen_fd, (struct sockaddr *)&client_address, &client_length);
        if (fork() == 0)
        {
            echo(connect_fd, &client_address);
            exit(0);
        }
        close_e(connect_fd);
    }
    close_e(listen_fd);
    return 0;
}