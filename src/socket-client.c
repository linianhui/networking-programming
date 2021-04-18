#include "cnp.h"

void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];

    get_local_prompt(connect_fd, local_prompt);
    log_debug("%s ", local_prompt);

    while (1)
    {
        // 读取用户输入，阻塞
        bzero(send_buf, sizeof(send_buf));
        fgets(send_buf, BUFFER_SIZE, input);
        send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);

        // 接收server响应，阻塞
        bzero(recv_buf, sizeof(recv_buf));
        recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);

        // 打印server响应
        bzero(remote_prompt, sizeof(remote_prompt));
        get_remote_prompt(connect_fd, remote_prompt);
        log_debug("%s %s", remote_prompt, recv_buf);

        // 打印用户输入提示符
        bzero(local_prompt, sizeof(local_prompt));
        get_local_prompt(connect_fd, local_prompt);
        log_debug("%s ", local_prompt);
    }
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);

    cli(stdin, connect_fd);

    exit(0);
}