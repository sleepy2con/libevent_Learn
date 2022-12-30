#include "Factory.h"
using namespace std;
Factory::Factory():ipAndPort(new st_ipAndPort)
{
	Config* config = new Config("./config.json");
	config->getConfig(*ipAndPort);
	cout << "\n"<<ipAndPort->ipAddr << ipAndPort->port;
	
	MainLoop();
}

void Factory::MainLoop()
{
}
