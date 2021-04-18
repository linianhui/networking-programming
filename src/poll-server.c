#include "cnp.h"
#include <poll.h>

void echo(int connect_fd)
{
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    int recv_size;

    if ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        send_e(connect_fd, buf, recv_size + 1, 0);
    }
}

int main(int argc, char *argv[])
{
    int listen_fd = socket_create_bind_listen(argc, argv);

    int max_fd = 1025;
    struct pollfd poll_fd_array[max_fd];

    // 添加listen_fd, 关注POLLIN事件
    poll_fd_array[0].fd = listen_fd;
    poll_fd_array[0].events = POLLIN;

    int fd_count = 1;

    while (1)
    {
        // 每次重新复制poll_fd_array到kernel
        poll(poll_fd_array, fd_count, -1);

        // 当listen_fd可读时，把获取的连接的fd放入poll_fd_array
        if (poll_fd_array[0].revents & POLLIN)
        {
            poll_fd_array[fd_count].fd = accept_e(listen_fd, NULL, NULL);
            poll_fd_array[fd_count].events = POLLIN;
            fd_count++;
        }

        // 循环检查connect_fd是否可读，可读就用echo处理
        // 从1开始是跳过0位置的listen_fd。
        for (int i = 1; i < fd_count; i++)
        {
            if (poll_fd_array[i].fd < 0)
            {
                continue;
            }
            // 如果可读，就echo处理
            if (poll_fd_array[i].revents & POLLIN)
            {
                echo(poll_fd_array[i].fd);
            }
        }
    }

    close_e(listen_fd);
    exit(0);
}