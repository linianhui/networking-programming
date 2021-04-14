#include "cnp.h"

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf("client srart pid %d\n", pid);

    int connect_fd = create_socket_ipv4_tcp();
    printf("connect_fd %d\n", connect_fd);

    struct sockaddr_in server_address = create_sockaddr_ipv4_port_from_args(argc, argv, "127.0.0.1");

    connect_e(connect_fd, (struct sockaddr *)&server_address, sizeof(server_address));

    char *server_ip_port = format_ipv4_port(&server_address);
    printf("connected to server %s\n", server_ip_port);

    char read_buffer[BUFFER_SIZE];
    char write_buffer[BUFFER_SIZE];
    while (1)
    {
        bzero(read_buffer, BUFFER_SIZE);
        bzero(write_buffer, BUFFER_SIZE);
        fgets(write_buffer, BUFFER_SIZE, stdin);
        send_e(connect_fd, write_buffer, strlen(write_buffer) + 1, 0);
        recv_e(connect_fd, read_buffer, BUFFER_SIZE, 0);
        printf("server echo:\n%s\n", read_buffer);
    }
    close_e(connect_fd);
}