/*
  This example program provides a trivial(琐碎的) server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed(冲洗、清除、刷新
  flush:将缓冲区中的数据强制输出到目标设备（如文件、屏幕、网络连接等），确保数据被真正写出而不是还停留在内存中。).

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/
//#include <iostream>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
//#include "pch.h"

int main(int argc, char** argv)
{
	// 核心,为各种平台定制特定的,基于事件的非阻塞io后端提供了一个抽象api,
	// 他能让你知道套接字何时准备好读取或写入,执行基本的超时功能,并检测操作系统信号

	struct event_base* base;
	struct evconnlistenner* listener;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	base = event_base_new();
	if (!base) {
		fprintf(stderr, "Could not initialize libevent!\n");
		return 1;
	}
	return 1;
}