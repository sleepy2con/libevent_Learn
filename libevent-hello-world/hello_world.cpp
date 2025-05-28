/*
  This example program provides a trivial(琐碎的) server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed(冲洗、清除、刷新
  flush:将缓冲区中的数据强制输出到目标设备（如文件、屏幕、网络连接等），确保数据被真正写出而不是还停留在内存中。).

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
//#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

static void listener_cb(struct evconnlistener* evutil_socket_t,
	struct sockaddr*, int socklen, void*);

static const int port = 9995;

int main(int argc, char** argv)
{
	// 核心,为各种平台定制特定的,基于事件的非阻塞io后端提供了一个抽象api,
	// 他能让你知道套接字何时准备好读取或写入,执行基本的超时功能,并检测操作系统信号

	struct event_base* base;
	struct evconnlistener* listener;
	struct event* signal_event;

	// 结构体被初始化为0，这表示sockaddr_in的sin_addr被设置为
	// INADDR_ANY，即 0.0.0.0，所有本地网卡。
	struct sockaddr_in sockaddr_in = { 0 }; 
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	base = event_base_new();
	printf("caijitest");
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}
	// 设置监听的地址端口
	sockaddr_in.sin_family = AF_INET;
	sockaddr_in.sin_port = htons(port);
	listener = evconnlistener_new_bind(base, listener_cb, (void*)base,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, -1,
		(struct sockaddr*)&sockaddr_in,
		sizeof(sockaddr_in));
	if (!listener)
	{
		std::cout << "could not create a listener\n";
		return 1;
	}
	return 1;
}