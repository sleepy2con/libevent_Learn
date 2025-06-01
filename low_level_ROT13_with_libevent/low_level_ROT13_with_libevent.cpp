// low_level_ROT13_with_libevent.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <event2/event.h>
#include <assert.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#endif

const int MAX_LINE = 16384;
const int FD_SETSIZE_1024 = 1024;

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

struct fd_state* alloc_fd_state(struct event_base* base, evutil_socket_t fd)
{
    //struct fd_state* state = (fd_state*)malloc(sizeof(struct fd_state));
    struct fd_state* state = new struct fd_state;
    if (!state)
        return nullptr;
    state->read_event = event_new(base, fd, EV_READ | EV_PERSIST, do_read, state);
    if (!state->read_event)
    {
        delete state;
        return nullptr;
    }
    state->write_event = event_new(base, fd, EV_WRITE | EV_PERSIST, do_write, state);
    if (!state->write_event)
    {
        event_free(state->read_event);
        delete state;
        return nullptr;
    }
    state->buffer_used = state->n_written = state->write_upto = 0;
    assert(state->write_event);
    return state;
}

void free_fd_state(struct fd_state* state)
{
    event_free(state->read_event);
    event_free(state->write_event);
    delete state;
}

void do_read(evutil_socket_t fd, short events, void* args)
{
    struct fd_state* state = (fd_state*)args;
    char buf[1024];
    // ssize_t,一律用int
    int result;
    while (1)
    {
        assert(state->write_event);
        result = recv(fd, buf, sizeof(buf), 0);
        if (result <= 0)
            break;
        for (int i = 0; i < result; i++)
        {
            if (state->buffer_used < sizeof(state->buffer))
            {
                state->buffer[state->buffer_used++] = rot13_char(buf[i]);
            }
            if (buf[i] == '\n')
            {
                assert(state->write_event);
                event_add(state->write_event, nullptr);
                state->write_upto = state->buffer_used;
            }
        }
        if (result == 0)
        {
            free_fd_state(state);
        }
        else if (result < 0)
        {
            if (errno == EAGAIN)
                return;
            std::cerr << "recv";
            free_fd_state(state);
        }
    }
}
void do_write(evutil_socket_t fd, short events, void* args)
{
    struct fd_state* state = (fd_state*)args;
    while (state->n_written < state->write_upto)
    {
        size_t result = send(fd, state->buffer + state->n_written, state->write_upto - state->n_written, 0);
        if (result < 0)
        {
            int err = WSAGetLastError();
            if (errno == EAGAIN) // XXXX use evutil macro
                return;
            free_fd_state(state);
            return;
        }
        assert(result != 0);
        state->n_written += result;
    }
    if (state->n_written == state->buffer_used)
        state->n_written = state->write_upto = state->buffer_used = 0;
    event_del(state->write_event);
}
void do_accept(evutil_socket_t listener, short events, void* arg)
{
    //std::cout << "do_accept was called";
    struct event_base* base = static_cast<event_base*>(arg);
    struct sockaddr_storage ss;
    socklen_t slen = sizeof(ss);
    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    std::cout << "fd" << fd << " was accept" << std::endl;
    if (fd < 0)
    {
        std::cerr << "accept";
    }
    else if (fd > FD_SETSIZE_1024)
    {
        closesocket(fd);
    }
    else {
        struct fd_state* state;
        evutil_make_socket_nonblocking(fd);
        state = alloc_fd_state(base, fd);
        assert(state);
        assert(state->write_event);
        event_add(state->read_event, nullptr);
    }
}

void run()
{
    evutil_socket_t listener;
    struct sockaddr_in sin;
    struct event_base* base;
    struct event* listener_event;
    base = event_base_new();
    if (!base)
        return;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(40713);
    listener = socket(AF_INET, SOCK_STREAM, 0);
    //if (listener < 0) {
    //    std::cerr<<"listener";
    //    return;
    //}
    evutil_make_socket_nonblocking(listener);

    int one = 1;
#ifdef _WIN32
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));
#else
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
#endif
    if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0)
    {
        std::cerr << "bind" << WSAGetLastError();
        return;
    }
    // 16是等待连接队列最大长度，表示最多允许16个客户端排队accept
    if (listen(listener, 16) < 0)
    {
        std::cerr << "listener";
        return;
    }
    listener_event = event_new(base, listener, EV_READ | EV_PERSIST, do_accept, (void*)base);
    event_add(listener_event, NULL);
    event_base_dispatch(base);
}

int main()
{
#ifdef _WIN32
    // 存储使用winsock时初始化需要的数据
    WSADATA wsa_data;
    // 调用WSAStartup需要传入Winsock 版本号。
    int ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (ret != 0) {
        std::cerr << "WSAStartup failed: " << ret << std::endl;
        return 1;
    }
#endif
    setvbuf(stdout, nullptr,_IONBF, 0);
    run();

    return 0;
}

