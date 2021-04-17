#include "cnp.h"

void cli(FILE *input, int connect_fd)
{
    int input_fd = fileno(input);
    int max_fd;
    fd_set set;

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];

    FD_ZERO(&set);

    ssize_t recv_size = 0;

    get_local_prompt(connect_fd, local_prompt);
    printf_flush("%s ", local_prompt);

    while (1)
    {
        FD_SET(input_fd, &set);
        FD_SET(connect_fd, &set);
        max_fd = imax(input_fd, connect_fd) + 1;

        select(max_fd, &set, NULL, NULL, NULL);

        bzero(recv_buf, sizeof(recv_buf));
        bzero(send_buf, sizeof(send_buf));
        bzero(local_prompt, sizeof(local_prompt));
        bzero(remote_prompt, sizeof(remote_prompt));

        // 读取用户输入
        if (FD_ISSET(input_fd, &set))
        {
            if (fgets(send_buf, BUFFER_SIZE, input) == NULL)
            {
                return;
            }
            send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
        }

        // 接收server响应
        if (FD_ISSET(connect_fd, &set))
        {
            recv_size = recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
            if (recv_size == 0)
            {
                printf_flush("recv server FIN");
                return;
            }
            get_remote_prompt(connect_fd, remote_prompt);
            printf_flush("%s %s", remote_prompt, recv_buf);

            get_local_prompt(connect_fd, local_prompt);
            printf_flush("%s ", local_prompt);
        }
    }
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);

    cli(stdin, connect_fd);

    exit(0);
}