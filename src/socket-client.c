#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 128

int main(int argc, char **argv)
{
    int connect_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));
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