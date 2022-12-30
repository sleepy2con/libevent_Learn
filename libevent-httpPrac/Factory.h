#pragma once
#include "pub.h"
#include "Config.h"
class Factory
{
public:
	Factory();
private:
	void MainLoop();	// eventBase
	void ifDefWin32();

	st_ipAndPort* ipAndPort;

};

