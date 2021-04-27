#include "config.h"

Config::Config(std::string cfgPath) {
	this->cfgPath = cfgPath;
}

void Config::read() {
	mINI::INIFile file(cfgPath);

	if (file.read(iniStruct)) {
		if (iniStruct.has("CONFIG")) {
			auto& configCollection = iniStruct["CONFIG"];

			if (configCollection.has("EXECUTABLE"))
				exe = configCollection["EXECUTABLE"];
			else
				throw std::runtime_error("Invalid config! \"EXECUTABLE\" value is missing!");

			if (configCollection.has("BASE_ADDRESS"))
				baseAddr = configCollection["BASE_ADDRESS"];
			else
				throw std::runtime_error("Invalid config! \"BASE_ADDRESS\" value is missing!");

			if (configCollection.has("ATTACH"))
				attachOn = StringToBool(configCollection["ATTACH"]);
			else
				attachOn = false;

			if (configCollection.has("REFRESH"))
			{
				auto _fps = atoi(configCollection["REFRESH"].c_str());

				switch (_fps)
				{
				default:
					hzRefresh = 16;
					break;
				case 120:
					hzRefresh = 8;
					break;
				case 240:
					hzRefresh = 4;
					break;
				}
			}

			else
				hzRefresh = 16;

			if (configCollection.has("BIG_ENDIAN"))
				bEndian = StringToBool(configCollection["ATTACH"]);
			else
				bEndian = false;
		}
		else
			throw std::runtime_error("Invalid config! \"[CONFIG]\" table is missing!");
	}
	else
		throw std::runtime_error("Unable to read \"config.ini\"! The file is either missing or corrupted.");
}

bool Config::StringToBool(std::string str)
{
	std::string out = str;
	std::transform(out.begin(), out.end(), out.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return out == "true" ? true : false;
}