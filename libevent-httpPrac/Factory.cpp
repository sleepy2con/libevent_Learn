#include "Factory.h"
using namespace std;
Factory::Factory():ipAndPort(new st_ipAndPort)
{
	Config* config = new Config("./config.json");
	config->getConfig(*ipAndPort);
	//cout << "\n"<<ipAndPort->ipAddr << ipAndPort->port;
	
	MainLoop();
}

void Factory::MainLoop()
{
	ifDefWin32();
	struct event_base* base;
	base = event_base_new();
	evutil_socket_t fd;
	event_callback_fn cb;
	struct event* event_prac = event_new(base, EV_PERSIST, 1, [](
		evutil_socket_t, short, void* arg) {
			cout << "event_prac's callback fun is executed,hhh\n";
		}, 0);
	struct timeval one_sec = { 1, 0 };
	event_add(event_prac, &one_sec);
	event_base_dispatch(base);
}

void Factory::ifDefWin32()
{
#ifdef _WIN32
	{
		WORD wVersionRequested;
		WSADATA wsaData;
		wVersionRequested = MAKEWORD(2, 2);
		WSAStartup(wVersionRequested, &wsaData);
	}
#else
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		ret = 1;
		goto err;
	}
#endif
}
