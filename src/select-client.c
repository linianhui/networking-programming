#include "cnp.h"

void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];

    ssize_t recv_size = 0;

    int input_fd = fileno(input);
    int max_fd = imax(input_fd, connect_fd) + 1;
    fd_set read_fd_set;

    log_stdin_prompt();

    while (1)
    {
        // 每次都需要重新设置，因为select返回时会重置read_fd_set
        FD_ZERO(&read_fd_set);
        FD_SET(input_fd, &read_fd_set);
        FD_SET(connect_fd, &read_fd_set);

        // 获取可读的fd，阻塞
        select(max_fd, &read_fd_set, NULL, NULL, NULL);

        // 读取用户输入，非阻塞
        if (FD_ISSET(input_fd, &read_fd_set))
        {
            bzero(send_buf, sizeof(send_buf));
            if (fgets(send_buf, BUFFER_SIZE, input) == NULL)
            {
                continue;
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
                close_e(connect_fd);
                continue;
            }
            // 打印用户输入提示符
            log_stdin_prompt();
        }
    }
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);
    cli(stdin, connect_fd);
    return 0;
}