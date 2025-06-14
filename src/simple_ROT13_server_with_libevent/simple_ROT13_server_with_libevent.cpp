

#include "Server.h"
#include <iostream>


//void do_read(evutil_socket_t fd, short events, void* arg);
//void do_write(evutil_socket_t fd, short events, void* arg);

int main()
{
	setvbuf(stdout, NULL, _IONBF, 0);

	Server server;
	server.run();
	return 0;
}
