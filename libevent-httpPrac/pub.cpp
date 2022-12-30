#include "pub.h"
#include <iostream>
using namespace std;

int loadConfig(cJSON& obj,const char* str)
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
	cout << cJSON_PrintUnformatted(&obj);
	return success;
}
