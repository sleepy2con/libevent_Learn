#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <evhttp.h>
#include <event.h>
#include <string.h>
#include "cJSON.h"
enum
{
	success,
	error
};

struct st_ipAndPort
{
	std::string ipAddr;
	int port;
	st_ipAndPort operator=(const st_ipAndPort& param)
	{
		if (this != &param)
		{
			this->ipAddr = param.ipAddr;
			this->port = param.port;
		}
		return *this;
	}
};

int loadConfig(cJSON& obj,const char* str = nullptr);