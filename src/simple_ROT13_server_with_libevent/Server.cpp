#include "Server.h"
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

void readcb(struct bufferevent* bev, void* ctx) {
	evbuffer* input, * output;
	char* line;
	size_t n;
	input = bufferevent_get_input(bev);
	output = bufferevent_get_output(bev);
	while ((line = evbuffer_readln(input, &n, EVBUFFER_EOL_LF)))
	{
		for (int i = 0; i < n; i++)
		{
			line[i] = rot13_char(line[i]);
		}
		evbuffer_add(output, line, n);
		evbuffer_add(output, "\n", 1);
		free(line);
	}
	if (evbuffer_get_length(input) >= MAX_LINE)
	{
		char buf[1024];
		while (evbuffer_get_length(input))
		{
			int n = evbuffer_remove(input, buf, sizeof(buf));
			for (int i = 0; i < n; i++)
			{
				buf[i] = rot13_char(buf[i]);
			}
			evbuffer_add(output, buf, n);
		}
		evbuffer_add(output, "\n", 1);
	}
}

void errorcb(struct bufferevent* bev, short error, void* ctx) {
	if (error & BEV_EVENT_EOF)
	{
	}
	else if (error & BEV_EVENT_ERROR)
	{
	}
	else if (error & BEV_EVENT_TIMEOUT)
	{
	}
	bufferevent_free(bev);
}

void do_accept(evutil_socket_t listener, short event, void* arg) {
	struct event_base* base = static_cast<event_base*>(arg);
	sockaddr_storage ss;
	socklen_t slen = sizeof(ss);
	int fd = accept(listener, (sockaddr*)&ss, &slen);
	if (fd < 0)
		std::cerr << "accept";
	else if (fd > FD_SETSIZE_1024)
		closesocket(fd);
	else {
		bufferevent* bev;
		evutil_make_socket_nonblocking(fd);
		bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(bev, readcb, nullptr, errorcb, nullptr);
		bufferevent_setwatermark(bev, EV_READ, 0, MAX_LINE);
		bufferevent_enable(bev, EV_READ | EV_WRITE);
	}
}

Server::Server()
{
#ifdef _WIN32
	// 存储使用winsock时初始化需要的数据
	WSADATA wsa_data;
	// 调用WSAStartup需要传入Winsock 版本号。
	WSAStartup(0x0201, &wsa_data);
#endif
}

void Server::run(void) {
	evutil_socket_t  listener;

	struct event_base* base;
	struct event* listener_event;
	base = event_base_new();
	if (!base)
		return;

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(40713);
	listener = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(listener);
	int one = 1;
#ifdef _WIN32
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));
#else
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
#endif
	if (bind(listener, (sockaddr*)&sin, sizeof(sin)) < 0)
	{
		std::cerr << "bind";
		return;
	}
	if (listen(listener, 16) < 0)
	{
		std::cerr << "listen";
		return;
	}
	listener_event = event_new(base, listener, EV_READ | EV_PERSIST,
		do_accept, (void*)base);
	event_add(listener_event, nullptr);
	event_base_dispatch(base);

}

