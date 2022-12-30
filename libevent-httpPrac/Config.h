#pragma once
#include "pub.h"

class Config
{
public:
	Config(const char* str = nullptr);
	void getConfig(st_ipAndPort& param_st);

private:
	void executeLoadConfig(const char* str = nullptr);
	int loadConfig(cJSON& obj, const char* str = nullptr);
	st_ipAndPort* m_st_ipAndPort;
};

