// windows上多进程处理客户端连接的ROT13服务器
// froking_ROT13_server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#include <thread>
#endif

const int MAX_LINE = 16384;

// 简单的文本加密
char rot13_char(char c)
{
	if ((c >= 'a' & c <= 'm') || (c >= 'A' && c <= 'M'))
		return c += 13;
	else if ((c >= 'n' & c <= 'z') || (c >= 'N' && c <= 'Z'))
		return c -= 13;
	return c;
}

void child(int fd)
{
	char outbuf[MAX_LINE + 1];
	size_t outbuf_used = 0;
	size_t result;
	while (1)
	{
		char ch;
		result = recv(fd, &ch, 1, 0);
		if (result == 0)
			break;
		else if (result == -1)
		{
			std::cerr << "read" << WSAGetLastError() << std::endl;
			break;
		}
		if (outbuf_used < sizeof(outbuf))
		{
			outbuf[outbuf_used++] = rot13_char(ch);
		}
		if (ch == '\n')
		{
			send(fd, outbuf, outbuf_used, 0);
			outbuf_used = 0;
			continue;
		}
	}
}
void run()
{
	int listener;
	struct sockaddr_in sin;
	sin.sin_family = AF_INET; // address family AF,
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(40713);
#ifdef _WIN32
	// 存储使用winsock时初始化需要的数据
	WSADATA wsa_data;
	// 调用WSAStartup需要传入Winsock 版本号。
	WSAStartup(0x0201, &wsa_data);
#endif

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if (listener == INVALID_SOCKET) {
		std::cerr << "socket listener creation failed: " << WSAGetLastError() << std::endl;
		return;
	}
	int one = 1;
	// 启用 socket 的 SO_REUSEADDR 选项，允许端口重用，以避免“Address already in use”错误
#ifdef _WIN32
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one));
#else
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
#endif

	if (bind(listener, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		std::cout << "bind" << WSAGetLastError() << std::endl;
		return;
	}
	if (listen(listener, 16) < 0)
	{
		std::cout << "listen";
		return;
	}

	while (1)
	{
		struct sockaddr_storage ss;
		socklen_t slen = sizeof(ss);
		int fd = accept(listener, (struct sockaddr*)&ss, &slen);
		if (fd < 0)
			std::cout << "accept";
		else {
			// 父进程中，fork的返回值是子进程的id，子进程中，fork的返回值是0
#ifdef __linux__
			if (fork() == 0)
			{
				child(fd);
				exit(0);
			}
#endif
			std::thread([fd]() {
				child(fd); }).detach();
		}
	}

}

// telnet localhost 40713
int main()
{
	run();
	return 0;
}


