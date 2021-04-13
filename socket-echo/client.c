#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

int main(int argc, char **argv)
{
    int connect_fd;
    
    struct sockaddr_in server_address;

    connect_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bzero(&server_address, sizeof server_address);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr));

    connect(connect_fd, (struct sockaddr *)&server_address, sizeof(server_address));

    char read_buffer[BUFFER_SIZE];
    char write_buffer[BUFFER_SIZE];
    while (1)
    {
        bzero(read_buffer, BUFFER_SIZE);
        bzero(write_buffer, BUFFER_SIZE);
        fgets(write_buffer, BUFFER_SIZE, stdin);
        write(connect_fd, write_buffer, strlen(write_buffer) + 1);
        read(connect_fd, read_buffer, BUFFER_SIZE);
        printf("server echo:\n%s", read_buffer);
    }
    close(connect_fd);
}