// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVC专用的编译器指令，在链接阶段，自动添加名为 Ws2_32.lib 的库文件，不需要你手动去项目设置里添加
#pragma comment(lib, "Ws2_32.lib") // 配合ws2tcpip.h；
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
#endif


#endif //PCH_H
