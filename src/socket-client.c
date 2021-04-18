#include "cnp.h"

void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];

    while (1)
    {
        // 打印用户输入提示符
        log_stdin_prompt();

        // 读取用户输入，阻塞
        bzero(send_buf, sizeof(send_buf));
        fgets(send_buf, BUFFER_SIZE, input);

        send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);

        // 接收server响应，阻塞
        bzero(recv_buf, sizeof(recv_buf));
        recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
    }
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);
    cli(stdin, connect_fd);
    return 0;
}