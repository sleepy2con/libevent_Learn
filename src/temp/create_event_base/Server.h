#ifndef SERVER_H
#define SERVER_H

#include <event2/event.h>
//#include <event2/buffer.h>
//#include <event2/bufferevent.h>

#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#include <iostream>

class Server
{
public:
	Server();
	void run();
	void printCurBackend();
	~Server();
private:
	event_base* m_base;
};

#endif
