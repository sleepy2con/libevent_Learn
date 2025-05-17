// block_io_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
//#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
//#include <windows.h>  // For Sleep (替代 usleep，如果需要)
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "")
int main()
{

	/*const char query[] = 
		"GET / HTTP/1.0\r\n"
		"Host: www.baidu.com\r\n"
		"\r\n";
	const char hostname[] = "www.baidu.com";
	struct sockaddr_in sin;
	struct hostent *h;
	const char* cp;
	int fd;
	ssize_t n_written, remaining;*/

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
