// block_io_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <stdio.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
#endif

// 为了避免每次退出都得调用WSACleanup
struct WSA_wraper
{
	WSADATA wsadata; // 存储初始化信息
	bool valid = false;
	WSA_wraper() {
		// return 0成功
		// windows必须借此初始化Winsock 库
		int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
		valid = (result == 0);
		if (!valid)
		{
			std::cout << "WSAStartup failed: " << result << std::endl;
		}
	};
	~WSA_wraper() { WSACleanup(); };
};


//#include <windows.h>  // For Sleep (替代 usleep，如果需要)
int main()
{
	// wchar_t 宽字符，L是宽字符前缀，表示后面的字符串是宽字符（wchar_t）类型的字符串，而不是普通的多字节（char）字符串。
	const wchar_t  localhost_str[] = L"127.0.0.1";
	const u_short listen_port = 8080;

	char path[MAX_PATH] = { 0 };
	// 获得当前程序名称
	GetModuleFileNameA(nullptr, path, MAX_PATH);
	//std::cout << path << std::endl;

	// 初始化 Winsock 库.Windows 上必须手动初始化 Winsock 库，Linux/Unix 系统不需要
	WSA_wraper wsa;
	if (!wsa.valid) return 1;
	//std::cout << "WSAStartup 成功" << std::endl;
	// 创建Socket，一个文件描述符/句柄
	int listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "socket failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;	// 用来指定地址类型（也叫地址族）
	// 主机字节序：有的机器是小端序（Intel x86 是小端），有的是大端序
	// 网络字节序：网络协议规定是 大端序（高位字节在前）
	service.sin_port = htons(listen_port); // 把主机字节序（Host Byte Order）的 16 位整数转换成网络字节序（Network Byte Order）
	InetPton(AF_INET, localhost_str, &service.sin_addr);
	if (bind(listenSocket, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "Bind failed: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		return 1;
	}
	// backlog 表示监听队列的最大长度，表示操作系统允许挂起（排队等待）的未处理连接请求的最大数目
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "listen failed:" << WSAGetLastError() << "\n";
		return 1;
	}
	std::cout << "waiting for client to connect..." << std::endl;

	const int msgSize = 512;
	char recvBuffer[msgSize];
	char sendBuffer[msgSize];
	while (1)
	{
		//accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen); 是用于接收客户端连接请求的系统调用，\
		// 它从监听 socket 中取出一个已经完成三次握手的连接，并返回一个新的 socket 文件描述符。
		// addr：传出参数，客户端地址会被填充进这个结构体（如 sockaddr_in
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		if (clientSocket == -1) {
			std::cout << "Accept clientSocket failed: " << WSAGetLastError() << std::endl;
			closesocket(listenSocket);
			return 1;
		}
		std::cout << "\nA new client accepted!" << std::endl;
		while (1)
		{
			// 正整数：表示实际收到的字节数
			int recvResult = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
			if (recvResult > 0) {
				recvBuffer[recvResult] = '\0';
				if (strncmp(recvBuffer, "exit",4) == 0) {
					std::cout << "Client requested to close connection." << std::endl;
					break;
				}
				std::cout << "Client says: " << recvBuffer << std::endl;
				// 你可以在这里输入要回复给客户端的内容
				std::cout << "Enter reply: ";
				std::cin.getline(sendBuffer, sizeof(sendBuffer));
				//std::cout << "server send: " << sendBuffer << std::endl;
				// 发送回复
				send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
			}
			else {
				std::cout << "Client connect failed." << std::endl;
				break;
			}
		}
		closesocket(clientSocket);
	}

	//  关闭 sockets
	closesocket(listenSocket);
	std::cin.get();
	return 1;
}


