#include "cnp.h"

void echo(int connect_fd, struct sockaddr *cliaddr)
{
    pid_t pid = getpid();
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    char client_ip_port[IP_PORT_STRING_SIZE];
    get_ip_port(cliaddr, client_ip_port);

    printf_flush("\nclient %s connected\n", client_ip_port);
    printf_flush("server pid %d connect_fd %d handler\n", pid, connect_fd);

    int recv_size;
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];
    while ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        bzero(local_prompt, sizeof(local_prompt));
        bzero(remote_prompt, sizeof(remote_prompt));

        get_remote_prompt(connect_fd, remote_prompt);
        printf_flush("%s %s", remote_prompt, buf);

        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        if (send_e(connect_fd, buf, recv_size + 1, 0) > 0)
        {
            get_local_prompt(connect_fd, remote_prompt);
            printf_flush("%s %s", remote_prompt, buf);
        }
    }
    printf_flush("server pid %d connect_fd %d close connect\n", pid, connect_fd);
    close_e(connect_fd);
}

int main(int argc, char *argv[])
{
    pid_t pid = getpid();
    printf_flush("server srart pid %d\n", pid);

    int listen_fd = create_socket();
    printf_flush("listen_fd %d\n", listen_fd);

    struct sockaddr_in servaddr;
    init_sockaddr_from_args(&servaddr, argc, argv, "0.0.0.0");

    bind_e(listen_fd, (sa *)&servaddr, sizeof(servaddr));
    listen_e(listen_fd, 10);

    char server_ip_port[IP_PORT_STRING_SIZE];
    get_ip_port((sa *)&servaddr, server_ip_port);
    printf_flush("listen on %s \nwaiting for client connect...\n", server_ip_port);

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