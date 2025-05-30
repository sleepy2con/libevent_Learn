// block_io_connect.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//对google.com去建立通信，发送一个http的get请求

#include <iostream>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#endif

int main()
{
#ifdef _WIN32
	// 存储使用winsock时初始化需要的数据
	WSADATA wsa_data;
	// 调用WSAStartup需要传入Winsock 版本号。
	WSAStartup(0x0201, &wsa_data);
#endif
	const char query[] = "GET / HTTP/1.0\r\n"
		"Host:www.google.com\r\n"
		//"Host:www.baidu.com\r\n"
		"\r\n";
	const char hostname[] = "www.google.com";
	struct sockaddr_in sin;
	struct hostent* h;
	const char* cp;
	int fd;
	size_t n_written, remaining;
	char buf[1024];
	const char* port = "80";
	struct addrinfo hints, * result, * p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	// 用于 将主机名（如 "google.com"）和服务名（如 "http"）转换为 addrinfo 结构体链表。
	int status = getaddrinfo(hostname, port, &hints, &result);
	if (status > 0)
	{
		std::cout << "getaddrinfo error," << gai_strerror(status);
		return 1;
	}
	bool found_avaliable_IPV4 = 0;
	for (p = result;p != nullptr;p = p->ai_next)
	{
		if (p->ai_family == AF_INET)
		{
			found_avaliable_IPV4 = 1;
			memcpy(&sin, p->ai_addr, sizeof(struct sockaddr_in));
			break;
		}
	}
	if (!found_avaliable_IPV4)
	{
		std::cerr << "not found avale";
		return 1;
	}
	// 方法已经弃用
	/*h = gethostbyname(hostname);
	if (!h)
	{
		std::cout << "找不到" << hostname << " ,错误是" << WSAGetLastError();
		return 1;
	}
	if (h->h_addrtype != AF_INET)
	{
		std::cerr << "no ipv6 support";
		return 1;
	}*/
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0)
	{
		std::cerr << "socket";
		return 1;
	}
	// 链接远程主机
	sin.sin_family = AF_INET;
	sin.sin_port = htons(80);
	//sin.sin_addr = *(struct in_addr*)h->h_addr;
	if (connect(fd, (struct sockaddr*)&sin, sizeof(sin)))
	{
		std::cerr << "connect";
		closesocket(fd);
		return 1;
	}
	cp = query;
	remaining = strlen(query);
	// 确保cp的内容被发完
	while (remaining)
	{
		// 无法确保每次send发送的字节，所以分批次发送
		// send返回值为每次发送成功的字节数
		// 每个 socket 都有一个发送缓冲区，大小有限（比如 8KB、16KB，甚至更小）。
		// 如果你一次发的数据大于这个缓冲区，send() 会：只发送部分数据；或者阻塞（阻塞 socket）；
		n_written = send(fd, cp, remaining, 0);
		if (n_written <= 0)
		{
			std::cerr << "send";
			return 1;
		}
		remaining -= n_written;
		cp += n_written;
	}
	// 获的回复
	while (1)
	{
		size_t result = recv(fd, buf, sizeof(buf), 0);
		if (result == 0)
			break;
		else if (result < 0)
		{
			std::cerr << "recv";
			closesocket(fd);
		}
		fwrite(buf, 1, result, stdout);
	}
	closesocket(fd);
	return 0;
}

