
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；

const int MAX_LINE = 16384;

void do_read(evutil_socket_t fd, short events, void* arg);
void do_write(evutil_socket_t fd, short events, void* arg);

char
rot13_char(char c)
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
void
readcb(struct bufferevent* bev, void* ctx) {

}
void
errorcb(struct bufferevent* bev, short error, void* ctx) {

}
void
do_accept(evutil_socket_t listener, short event, void* arg) {

}
void
run(void) {
	evutil_socket_t  listener;
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
	evutil_make_socket_nonblocking(listener);
	int one = 1;
#ifndef WIN32{
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one);
	}

#endif // !WIN32{}


}
int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);

	run();
	return 0;
}
