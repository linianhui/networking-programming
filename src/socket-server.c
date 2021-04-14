#include "cnp.h"

void echo(int connect_fd, struct sockaddr_in *client_address)
{
    pid_t pid = getpid();
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    char *client_ip_port = format_ipv4_port(client_address);

    printf("\nclient %s connected\n", client_ip_port);
    printf("server pid=%d handler\n", pid);

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
            printf("server pid=%d echo toupper:\n%s\n", pid, buffer);
        }
    }
    printf("server pid=%d close connect\n", pid);
}

int main()
{
    pid_t pid = getpid();
    printf("server srart pid=%d\n", pid);

    int listen_fd = create_socket_ipv4_tcp();

    struct sockaddr_in server_address = create_sockaddr_ipv4_port("0.0.0.0", SERVER_PORT);
    bind_e(listen_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    listen_e(listen_fd, 10);

    char *server_ip_port = format_ipv4_port(&server_address);
    printf("listen on %s \nwaiting for client connect...\n", server_ip_port);

    int connect_fd;
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (1)
    {
        accept_e(listen_fd, (struct sockaddr *)&client_address, &client_length);
        if (fork() == 0)
        {
            //close_e(listen_fd);
            echo(connect_fd, &client_address);
            exit(0);
        }
        close_e(connect_fd);
    }
    close_e(listen_fd);
    return 0;
}