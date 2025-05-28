// low_level_ROT13_with_libevent.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <event2/event.h>
const int MAX_LINE = 16384;

void do_read(evutil_socket_t fd, short events, void* args);
void do_write(evutil_socket_t fd, short events, void* args);

char rot13_char(char c)
{
    /* We don't want to use isalpha here; setting the locale would change
     * which characters are considered alphabetical. */
    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
        return c + 13;
    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
        return c - 13;
    else
        return c;
}
struct fd_state {
    char buffer[MAX_LINE];
    size_t buffer_used;
    size_t n_written;
    size_t write_upto;
    struct event* read_event;
    struct event* write_event;
};

void do_read(evutil_socket_t fd, short events, void* args)
{

}
void do_write(evutil_socket_t fd, short events, void* args)
{

}
void do_accept(evutil_socket_t, short events, void* arg)
{

}

void run()
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base* base;
    struct event* listener_event;
    base = event_base_new();
}

int main()
{
    run();
    return 0;
}

