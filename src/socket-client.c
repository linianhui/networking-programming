#include "cnp.h"

int main(int argc, char *argv[])
{
    int connect_fd = create_socket_ipv4_tcp();

    struct sockaddr_in server_address = create_sockaddr_ipv4_port("127.0.0.1", SERVER_PORT);

    printf("client srart\n");

    connect(connect_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    char *server_ip = inet_ntoa(server_address.sin_addr);
    in_port_t server_port = server_address.sin_port;
    printf("connected to server %s:%d ...\n", server_ip, server_port);

    char read_buffer[BUFFER_SIZE];
    char write_buffer[BUFFER_SIZE];
    while (1)
    {
        bzero(read_buffer, BUFFER_SIZE);
        bzero(write_buffer, BUFFER_SIZE);
        fgets(write_buffer, BUFFER_SIZE, stdin);
        send(connect_fd, write_buffer, strlen(write_buffer) + 1, 0);
        recv(connect_fd, read_buffer, BUFFER_SIZE, 0);
        printf("server echo:\n%s\n", read_buffer);
    }
    close(connect_fd);
}