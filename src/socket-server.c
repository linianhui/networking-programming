#include "cnp.h"

void echo(int connect_fd)
{
    pid_t pid = getpid();

    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    int recv_size;

    // 读取接收的数据，阻塞
    while ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        send_e(connect_fd, buf, recv_size + 1, 0);
    }
    log_debug("server pid %d connect_fd %d close connect\n", pid, connect_fd);
}

int main(int argc, char *argv[])
{
    int listen_fd = socket_create_bind_listen(argc, argv);
    int connect_fd;

    while (1)
    {
        // 获取已建立的连接，阻塞。
        connect_fd = accept_e(listen_fd, NULL, NULL);
        if (fork() == 0)
        {
            echo(connect_fd);
            exit(0);
        }
        close_e(connect_fd);
    }

    close_e(listen_fd);
    exit(0);
}