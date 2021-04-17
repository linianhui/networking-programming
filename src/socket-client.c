#include "cnp.h"
void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];
    while (1)
    {
        bzero(recv_buf, sizeof(recv_buf));
        bzero(send_buf, sizeof(send_buf));
        bzero(local_prompt, sizeof(local_prompt));
        bzero(remote_prompt, sizeof(remote_prompt));

        get_local_prompt(connect_fd, local_prompt);
        printf_flush("%s ", local_prompt);

        fgets(send_buf, BUFFER_SIZE, input);
        send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
        recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);

        get_remote_prompt(connect_fd, remote_prompt);
        printf_flush("%s %s", remote_prompt, recv_buf);
    }
}

int main(int argc, char *argv[])
{
    register_signal();

    pid_t pid = getpid();
    printf_flush("client srart pid %d\n", pid);

    int connect_fd = create_socket();
    printf_flush("connect_fd %d\n", connect_fd);
    struct sockaddr_in servaddr;
    init_sockaddr_from_args(&servaddr, argc, argv, "127.0.0.1");

    connect_e(connect_fd, (sa *)&servaddr, sizeof(servaddr));

    char server_ip_port[IP_PORT_STRING_SIZE];
    get_ip_port((sa *)&servaddr, server_ip_port);
    printf_flush("connected to server %s\n", server_ip_port);

    cli(stdin, connect_fd);

    exit(0);
}