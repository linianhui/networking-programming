#include "cnp.h"

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

void cli(FILE *input, int connect_fd)
{
    char recv_buf[BUFFER_SIZE];
    char send_buf[BUFFER_SIZE];

    log_stdin_prompt();

    int event_count = 2;
    struct epoll_event event_array[event_count];

    int input_fd = fileno(input);
    int epoll_fd = epoll_create(event_count);

    epoll_ctl_add(epoll_fd, input_fd);
    epoll_ctl_add(epoll_fd, connect_fd);

    ssize_t recv_size = 0;
    int i, fd;
    uint32_t events;

    while (1)
    {
        // 获取可读的fd，阻塞
        epoll_wait(epoll_fd, event_array, event_count, -1);

        for (i = 0; i < 2; i++)
        {
            fd = event_array[i].data.fd;
            events = event_array[i].events;

            if (fd == connect_fd && (events & EPOLLIN))
            {
                bzero(recv_buf, sizeof(recv_buf));
                recv_size = recv_e(connect_fd, recv_buf, BUFFER_SIZE, 0);
                if (recv_size == 0)
                {
                    epoll_ctl_del(epoll_fd, connect_fd);
                }
                log_stdin_prompt();
                continue;
            }

            if (fd == input_fd && (events & EPOLLIN))
            {
                bzero(send_buf, sizeof(send_buf));
                if (fgets(send_buf, BUFFER_SIZE, input) == NULL)
                {
                    continue;
                }
                send_e(connect_fd, send_buf, strlen(send_buf) + 1, 0);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    int connect_fd = socket_create_connect(argc, argv);
    cli(stdin, connect_fd);
    return 0;
}