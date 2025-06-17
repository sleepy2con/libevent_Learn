#ifndef SERVER_H
#define SERVER_H

#include <event2/event.h>
//#include <event2/buffer.h>
//#include <event2/bufferevent.h>

#include <winsock2.h>
#include <ws2tcpip.h> // For getaddrinfo (recommended)
// MSVCר�õı�����ָ������ӽ׶Σ��Զ������Ϊ Ws2_32.lib �Ŀ��ļ�������Ҫ���ֶ�ȥ��Ŀ���������
#pragma comment(lib, "Ws2_32.lib") // ���ws2tcpip.h��
#include <iostream>

class Server
{
public:
	Server();
	void run();
	void printCurBackend();
	~Server();
private:
	event_base* m_base;
};

#endif
