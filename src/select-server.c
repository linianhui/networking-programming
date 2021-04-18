#include "cnp.h"

int echo(int connect_fd)
{
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));
    return socket_revc_and_send(connect_fd, buf);
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
        bitmap_loop(connect_fd_set, FD_SET(i, &read_fd_set));

        // 获取可读的fd，阻塞
        select(connect_fd_set->len, &read_fd_set, NULL, NULL, NULL);

        // 当listen_fd可读时，把获取的连接的fd放入connect_fd_set
        if (FD_ISSET(listen_fd, &read_fd_set))
        {
            bitmap_set(connect_fd_set, accept_e(listen_fd, NULL, NULL));
        }

        // 循环检查connect fd是否可读，可读就用echo处理
        bitmap_loop(
            connect_fd_set,
            if (FD_ISSET(i, &read_fd_set)) {
                if (echo(i) == 0)
                {
                    // 对方断开了连接，那么则移除select，并且关闭连接
                    bitmap_del(connect_fd_set, i);
                    close_e(i);
                }
            });
    }

    close_e(listen_fd);
    exit(0);
}