#ifndef SERVER_H
#define SERVER_H

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；

#include <iostream>
const int MAX_LINE = 16384;
const int FD_SETSIZE_1024 = 1024;

static char rot13_char(char c);

class Server
{
public:
	Server();
	void run();
	//void do_accept(evutil_socket_t listener, short event, void* arg);
	//void readcb(struct bufferevent* bev, void* ctx);
	//void errorcb(struct bufferevent* bev, short error, void* ctx);

};

#endif // ! SERVER_H