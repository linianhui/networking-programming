#include "cnp.h"
#include <sys/epoll.h>

int echo(int connect_fd)
{
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));
    return socket_revc_and_send(connect_fd, buf);
}

void epoll_ctl_add(int epoll_fd, int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_ctl_del(int epoll_fd, int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev);
}

void epoll_handler(int listen_fd)
{
    int epoll_fd = epoll_create(1024);
    epoll_ctl_add(epoll_fd, listen_fd);

    int index;
    int fd;
    uint32_t events;

    int event_count = 4;
    struct epoll_event event_array[event_count];

    while (1)
    {
        bzero(event_array, sizeof(event_array));

        // 每次返回指定数量的可读fd
        epoll_wait(epoll_fd, event_array, event_count, -1);
        log_socket_epoll_event(event_array, event_count);

        for (index = 0; index < event_count; index++)
        {
            fd = event_array[index].data.fd;
            if (fd < 0)
            {
                continue;
            }

            events = event_array[index].events;

            // 当listen_fd可读，把获取的连接的fd放入epoll_fd
            if (fd == listen_fd)
            {
                if (events & EPOLLIN)
                {
                    epoll_ctl_add(epoll_fd, accept_e(listen_fd, NULL, NULL));
                }
                continue;
            }

            // 当connect_fd可读时，交由echo处理
            if (events & EPOLLIN)
            {
                if (echo(fd) == 0)
                {
                    epoll_ctl_del(epoll_fd, fd);
                    close_e(fd);
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int listen_fd = socket_create_bind_listen(argc, argv);
    epoll_handler(listen_fd);
    return 0;
}