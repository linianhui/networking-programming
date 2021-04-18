#include "cnp.h"

void echo(int connect_fd)
{
    char buf[BUFFER_SIZE];
    int recv_size;

    while (1)
    {
        bzero(buf, sizeof(buf));
        // 读取接收的数据，阻塞
        recv_size = socket_revc_and_send(connect_fd, buf);
        if (recv_size == 0)
        {
            break;
        }
    }
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