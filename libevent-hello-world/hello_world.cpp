/*
  This example program provides a trivial(�����) server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed(��ϴ�������ˢ��
  flush:���������е�����ǿ�������Ŀ���豸�����ļ�����Ļ���������ӵȣ���ȷ�����ݱ�����д�������ǻ�ͣ�����ڴ��С�).

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
	// ����,Ϊ����ƽ̨�����ض���,�����¼��ķ�����io����ṩ��һ������api,
	// ��������֪���׽��ֺ�ʱ׼���ö�ȡ��д��,ִ�л����ĳ�ʱ����,��������ϵͳ�ź�

	struct event_base* base;
	struct evconnlistener* listener;
	struct event* signal_event;

	// �ṹ�屻��ʼ��Ϊ0�����ʾsockaddr_in��sin_addr������Ϊ
	// INADDR_ANY���� 0.0.0.0�����б���������
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
	// ���ü����ĵ�ַ�˿�
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