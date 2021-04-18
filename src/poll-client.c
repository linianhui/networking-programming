#include "cnp.h"
#include <poll.h>

void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];

    ssize_t recv_size = 0;

    get_local_prompt(connect_fd, local_prompt);
    printf_flush("%s ", local_prompt);

    int input_fd = fileno(input);
    int max_fd = imax(input_fd, connect_fd) + 1;
    struct pollfd poll_fd_array[2];

    // 添加input_fd, 关注POLLIN事件
    poll_fd_array[0].fd = input_fd;
    poll_fd_array[0].events = POLLIN;

    // 添加connect_fd, 关注POLLIN事件
    poll_fd_array[1].fd = connect_fd;
    poll_fd_array[1].events = POLLIN;

    while (1)
    {
        // 获取可读的fd，阻塞
        poll(poll_fd_array, max_fd, -1);

        // 读取用户输入，非阻塞
        if (poll_fd_array[0].revents & POLLIN)
        {
            bzero(send_buf, sizeof(send_buf));
            if (fgets(send_buf, BUFFER_SIZE, input) == NULL)
            {
                continue;
            }
            send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
        }
        // 接收server响应，非阻塞
        if (poll_fd_array[1].revents & POLLIN)
        {
            bzero(recv_buf, sizeof(recv_buf));
            recv_size = recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
            if (recv_size == 0)
            {
                printf("\nrecv server FIN\n");
                // 打印用户输入提示符
                bzero(local_prompt, sizeof(local_prompt));
                get_local_prompt(connect_fd, local_prompt);
                printf_flush("%s ", local_prompt);

                close_e(connect_fd);
                continue;
            }
            else
            {
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
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);

    cli(stdin, connect_fd);

    exit(0);
}