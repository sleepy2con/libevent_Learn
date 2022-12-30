#include "Config.h"
using namespace std;

Config::Config(const char* str):m_st_ipAndPort(new st_ipAndPort)
{
	executeLoadConfig(str);
}

void Config::getConfig(st_ipAndPort& param_st)
{
	param_st = *m_st_ipAndPort;
}

void Config::executeLoadConfig(const char* str)
{
	cJSON* config = new cJSON;
	//cout << cJSON_PrintUnformatted(&config);
	if (success != loadConfig(*config, str))
	{
		cout << "loadConfig error\n";
		return;
	}
	cJSON* tempJson = nullptr;
	tempJson = cJSON_GetObjectItem(config, "CommConf");
	//cout << "\n" << cJSON_Print(tempJson);

	m_st_ipAndPort->ipAddr = cJSON_GetObjectItem(tempJson, "localIP")->valuestring;
	m_st_ipAndPort->port = cJSON_GetObjectItem(tempJson, "port")->valueint;
	delete config;
}

int Config::loadConfig(cJSON& obj, const char* str)
{
	if (!str)
	{
		return error;
	}
	FILE* configFile = nullptr;
	//std::cout << "Current path is " << filesystem::current_path() << '\n';
	fopen_s(&configFile, str, "rb");
	if (!configFile)
	{
		cout << "fopen config.json filed\n";
		return error;
	}
	fseek(configFile, 0, SEEK_END);
	long configStrLen = ftell(configFile);
	//cout << configStrLen << endl;

	char* configStr = (char*)malloc(configStrLen + 1);
	fseek(configFile, 0, SEEK_SET);
	fread(configStr, 1, configStrLen, configFile);
	//configStr[configStrLen] = '\0';
	fclose(configFile);
	//cout << configStr;

	obj = *cJSON_Parse(configStr);
	//cout << cJSON_PrintUnformatted(&obj);
	return success;
}
