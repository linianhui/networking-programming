#include "cnp.h"

void echo(int connect_fd)
{
    pid_t pid = getpid();
    char buf[BUFFER_SIZE];
    bzero(buf, sizeof(buf));

    char client_ip_port[IP_PORT_STRING_SIZE];
    get_remote_ip_port(connect_fd, client_ip_port);

    log_debug("\nclient %s connected\n", client_ip_port);
    log_debug("server pid %d connect_fd %d handler\n", pid, connect_fd);

    int recv_size;
    char local_prompt[PROMPT_SIZE];
    char remote_prompt[PROMPT_SIZE];
    // 读取接收的数据，阻塞
    while ((recv_size = recv_e(connect_fd, buf, sizeof(buf), 0)) != 0)
    {
        bzero(remote_prompt, sizeof(remote_prompt));
        get_remote_prompt(connect_fd, remote_prompt);
        log_debug("%s %s", remote_prompt, buf);

        for (size_t i = 0; i < recv_size; i++)
        {
            buf[i] = toupper(buf[i]);
        }

        if (send_e(connect_fd, buf, recv_size + 1, 0) > 0)
        {
            bzero(local_prompt, sizeof(local_prompt));
            get_local_prompt(connect_fd, remote_prompt);
            log_debug("%s %s", remote_prompt, buf);
        }
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