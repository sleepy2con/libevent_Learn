#include "Server.h"
Server::Server()
{
#ifdef _WIN32
	// 存储使用winsock时初始化需要的数据
	WSADATA wsa_data;
	// 调用WSAStartup需要传入Winsock 版本号。
	WSAStartup(0x0201, &wsa_data);
#endif
}

void Server::run(void)
{
	//这是 Libevent 的核心。它为各种平台特定的、基于事件的非阻塞 IO 后端提供了
	// 一个抽象的 API。它可以让你知道套接字何时准备好读取或写入，执行基本的超时功能，并检测操作系统信号。
	event_config* tConfig = event_config_new();
	if(!tConfig)
	// 配置event_base在多线程下尽量充分利用指定数量的cpu,仅在使用IOCP的windows有效
	event_config_set_num_cpus_hint(tConfig, 2);
	//不要为 event_base 分配锁。设置此选项可能会节省一些锁定和释放 event_base 的时间，
	// 但会使从多个线程访问它变得不安全且无法正常工作
	event_config_set_flag(tConfig,EVENT_BASE_FLAG_NOLOCK);
	m_base = event_base_new_with_config(tConfig);
	event_config_free(tConfig);
	// 如果选择了一个后端你操作系统不支持的配置,base会初始化失败
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

