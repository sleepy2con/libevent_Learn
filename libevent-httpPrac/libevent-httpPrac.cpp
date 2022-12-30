

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#pragma comment(lib, "ws2_32.lib")
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else /* !_WIN32 */
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif /* _WIN32 */

#include "pub.h"
#include "Factory.h"
using namespace std;

#define BUF_MAX 1024*16

void WhatIDoAfterFileChanged(evutil_socket_t, short, void* arg);

int main()
{
	struct evhttp* http_server = NULL;
	short http_port = 8081;
	char http_addr[20] = "127.0.0.1";
	//event_base* base = event_init();

	// 监听文件变化事件
	struct event* ev_WatchFileChanged;

	Factory* factory = new Factory();


	return 0;
}


void WhatIDoAfterFileChanged(evutil_socket_t fd, short flag, void* arg)
{


}