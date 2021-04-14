#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define LISTEN_PORT 12345
#define BUFFER_SIZE 128

void echo(int connect_fd, struct sockaddr_in *client_address)
{
    pid_t pid = getpid();
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    char *client_ip = inet_ntoa(client_address->sin_addr);
    in_port_t client_port = client_address->sin_port;

    printf("\nclient %s:%d connected\n", client_ip, client_port);
    printf("server pid=%d handler\n", pid);

    int read_size;
    while ((read_size = recv(connect_fd, buffer, sizeof(buffer), 0)) != 0)
    {
        printf("client %s:%d send\n%s\n", client_ip, client_port, buffer);
        for (size_t i = 0; i < read_size; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        if (send(connect_fd, buffer, read_size + 1, 0) > 0)
        {
            printf("server pid=%d echo toupper:\n%s\n", pid, buffer);
        }
    }
    printf("server pid=%d close connect\n", pid);
}

int main()
{
    int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_fd < 0)
    {
        perror("socket error");
        return 1;
    }

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(LISTEN_PORT);
    if (bind(listen_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind error");
        return 2;
    }

    if (listen(listen_fd, 10) < 0)
    {
        perror("listen error");
        return 3;
    }

    pid_t pid = getpid();
    printf("server srart pid=%d port=%d \nwaiting for client connect...\n", pid, LISTEN_PORT);

    int connect_fd;
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);

    while (1)
    {
        if ((connect_fd = accept(listen_fd, (struct sockaddr *)&client_address, &client_length)) < 0)
        {
            perror("accept error");
            return 4;
        }
        if (fork() == 0)
        {
            close(listen_fd);
            echo(connect_fd, &client_address);
            exit(0);
        }
        close(connect_fd);
    }
    close(listen_fd);
    return 0;
}