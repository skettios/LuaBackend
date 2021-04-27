#ifndef CONFIG_H
#define CONFIG_H

#include <mIni/ini.h>
#include <iostream>

class Config
{
public:
	std::string exe;
	std::string baseAddr;
	bool attachOn;
	bool bEndian;
	int hzRefresh;

	Config(std::string cfgPath);

	void read();

private:
	std::string cfgPath;
	mINI::INIStructure iniStruct;

	bool StringToBool(std::string str);
};

#endif