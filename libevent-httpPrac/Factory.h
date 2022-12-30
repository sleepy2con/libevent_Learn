#pragma once
#include "pub.h"
#include "Config.h"
class Factory
{
public:
	Factory();
private:
	void MainLoop();	// eventBase


	st_ipAndPort* ipAndPort;

};

