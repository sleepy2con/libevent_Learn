// simpleSocket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	// 一个简单的socket程序
	// socket()创建监听socket
	//  bind()绑定IP + 端口
	//	listen()开始监听
	//	accept()接收一个客户端连接，得到新的clientSocket
	//	用clientSocket去send()和recv()

	// 测试 telnet 127.0.0.1 8080
	const wchar_t  localhost_str[] = L"127.0.0.1";
	const u_short listen_port = 8080;

	char path[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, path, MAX_PATH);
	std::cout << path << std::endl;
	WSADATA wsadata;
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0)
	{
		std::cout << "WSAStartup failed: " << result << std::endl;
		return 1;
	}
	// 创建socket
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		std::cout << "Socket failed: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}
	// 3. 绑定地址和端口
	sockaddr_in service;
	service.sin_family = AF_INET;
	//service.sin_addr.s_addr = InetPton("127.0.0.1"); // 本地回环
	service.sin_port = htons(listen_port); // 监听8080端口
	// 使用 InetPton 替换 inet_addr
	InetPton(AF_INET, localhost_str, &service.sin_addr);
	if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		std::cout << "Bind failed: " << WSAGetLastError() << "\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	
	// 4. 监听端口
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed: " << WSAGetLastError() << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Waiting for client to connect..." << std::endl;
	// 5. 接受客户端连接
	SOCKET clientSocket = accept(listenSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Accept clientSocket failed: " << WSAGetLastError() << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}
	std::cout << "Client connected!" << std::endl;

	char recvBuffer[512];
	char sendBuffer[512];
	while (true) {
		int recvResult = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
		if (recvResult > 0) {
			recvBuffer[recvResult] = '\0';
			std::cout << "Client says: " << recvBuffer << std::endl;

			// 你可以在这里输入要回复给客户端的内容
			std::cout << "Enter reply: ";
			std::cin.getline(sendBuffer, sizeof(sendBuffer));
			std::cout << "server send: " << sendBuffer << std::endl;
			// 发送回复
			send(clientSocket, sendBuffer, (int)strlen(sendBuffer), 0);
		}
		else if (recvResult == 0) {
			std::cout << "Client disconnected." << std::endl;
			break;  
		}
		else {
			std::cout << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}
	}

	//  关闭 sockets
	closesocket(clientSocket);
	closesocket(listenSocket);

	WSACleanup(); // 清理Winsock

	return 0;
}
