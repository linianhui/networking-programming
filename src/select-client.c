#include "cnp.h"

void cli(FILE *input, int connect_fd)
{
    int input_fd = fileno(input);
    int max_fd;
    fd_set read_fd_set;

    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];

    FD_ZERO(&read_fd_set);

    ssize_t recv_size = 0;

    get_local_prompt(connect_fd, local_prompt);
    printf_flush("%s ", local_prompt);

    while (1)
    {
        FD_SET(input_fd, &read_fd_set);
        FD_SET(connect_fd, &read_fd_set);
        max_fd = imax(input_fd, connect_fd) + 1;

        // 获取可读的fd，阻塞
        select(max_fd, &read_fd_set, NULL, NULL, NULL);

        // 读取用户输入，非阻塞
        if (FD_ISSET(input_fd, &read_fd_set))
        {
            bzero(send_buf, sizeof(send_buf));
            if (fgets(send_buf, BUFFER_SIZE, input) == NULL)
            {
                return;
            }
            send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
        }

        // 接收server响应，非阻塞
        if (FD_ISSET(connect_fd, &read_fd_set))
        {
            bzero(recv_buf, sizeof(recv_buf));
            recv_size = recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
            if (recv_size == 0)
            {
                printf_flush("recv server FIN");
                return;
            }

            // 打印server响应
            bzero(remote_prompt, sizeof(remote_prompt));
            get_remote_prompt(connect_fd, remote_prompt);
            printf_flush("%s %s", remote_prompt, recv_buf);

            // 打印用户输入提示符
            bzero(local_prompt, sizeof(local_prompt));
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