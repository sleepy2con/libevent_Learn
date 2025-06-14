#ifndef SERVER_H
#define SERVER_H

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVCר�õı�����ָ������ӽ׶Σ��Զ������Ϊ Ws2_32.lib �Ŀ��ļ�������Ҫ���ֶ�ȥ��Ŀ���������
#pragma comment(lib, "Ws2_32.lib") // ���ws2tcpip.h��

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