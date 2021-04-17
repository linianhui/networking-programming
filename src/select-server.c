#include "cnp.h"

void echo(int connect_fd)
{
    pid_t pid = getpid();
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    int recv_size;
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];
    if ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        bzero(remote_prompt, sizeof(remote_prompt));
        get_remote_prompt(connect_fd, remote_prompt);
        printf_flush("%s %s", remote_prompt, buf);

        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        if (send_e(connect_fd, buf, recv_size + 1, 0) > 0)
        {
            bzero(local_prompt, sizeof(local_prompt));
            get_local_prompt(connect_fd, remote_prompt);
            printf_flush("%s %s", remote_prompt, buf);
        }
    }
}

int main(int argc, char *argv[])
{
    int listen_fd = socket_create_bind_listen(argc, argv);

    bitmap *connect_fd_set = bitmap_init(1024);

    fd_set read_fd_set;

    while (1)
    {
        // 每次都需要重新设置，因为select返回时会重置read_fd_set
        FD_ZERO(&read_fd_set);
        FD_SET(listen_fd, &read_fd_set);
        bit_loop(connect_fd_set, FD_SET(i, &read_fd_set));

        // 获取可读的fd，阻塞
        select(connect_fd_set->len, &read_fd_set, NULL, NULL, NULL);

        // 当listen_fd可读时，把获取的连接的fd放入connect_fd_set
        if (FD_ISSET(listen_fd, &read_fd_set))
        {
            bitmap_set(connect_fd_set, accept_e(listen_fd, NULL, NULL));
        }

        // 循环检查connect fd是否可读，可读就用echo处理
        bit_loop(
            connect_fd_set,
            if (FD_ISSET(i, &read_fd_set)) {
                echo(i);
            });
    }

    close_e(listen_fd);
    exit(0);
}