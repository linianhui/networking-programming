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
    int connect_fd = socket_create_connect(argc, argv);

    cli(stdin, connect_fd);

    exit(0);
}