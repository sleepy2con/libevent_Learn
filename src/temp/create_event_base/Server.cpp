#include "Server.h"
Server::Server()
{
#ifdef _WIN32
	// �洢ʹ��winsockʱ��ʼ����Ҫ������
	WSADATA wsa_data;
	// ����WSAStartup��Ҫ����Winsock �汾�š�
	WSAStartup(0x0201, &wsa_data);
#endif
}

void Server::run(void)
{
	//���� Libevent �ĺ��ġ���Ϊ����ƽ̨�ض��ġ������¼��ķ����� IO ����ṩ��
	// һ������� API������������֪���׽��ֺ�ʱ׼���ö�ȡ��д�룬ִ�л����ĳ�ʱ���ܣ���������ϵͳ�źš�
	event_config* tConfig = event_config_new();
	if(!tConfig)
	// ����event_base�ڶ��߳��¾����������ָ��������cpu,����ʹ��IOCP��windows��Ч
	event_config_set_num_cpus_hint(tConfig, 2);
	//��ҪΪ event_base �����������ô�ѡ����ܻ��ʡһЩ�������ͷ� event_base ��ʱ�䣬
	// ����ʹ�Ӷ���̷߳�������ò���ȫ���޷���������
	event_config_set_flag(tConfig,EVENT_BASE_FLAG_NOLOCK);
	m_base = event_base_new_with_config(tConfig);
	event_config_free(tConfig);
	// ���ѡ����һ����������ϵͳ��֧�ֵ�����,base���ʼ��ʧ��
	if (!m_base)
	{
		std::cerr << "base";
		return;
	}
	printCurBackend();
	if (event_base_dispatch(m_base))
	{
		std::cerr << "event base loop";
		return;
	}

}

void Server::printCurBackend()
{
	const char** methods = event_get_supported_methods();
	printf("Starting Libevent %s.  Available methods are:\n",
		event_get_version());
	for (int i = 0; methods[i] != NULL; ++i) {
		printf("    %s\n", methods[i]);
	}
}

Server::~Server()
{
	event_base_loopbreak(m_base);
	if (m_base)
		event_base_free(m_base);
}

