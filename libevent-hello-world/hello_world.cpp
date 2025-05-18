/*
  This example program provides a trivial(�����) server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed(��ϴ�������ˢ��
  flush:���������е�����ǿ�������Ŀ���豸�����ļ�����Ļ���������ӵȣ���ȷ�����ݱ�����д�������ǻ�ͣ�����ڴ��С�).

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
	// ����,Ϊ����ƽ̨�����ض���,�����¼��ķ�����io����ṩ��һ������api,
	// ��������֪���׽��ֺ�ʱ׼���ö�ȡ��д��,ִ�л����ĳ�ʱ����,��������ϵͳ�ź�

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