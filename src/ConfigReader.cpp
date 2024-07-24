#include "ConfigReader.h"

//_____________________________________________________________________________
configReader::configReader(std::string configFilePath) {
	_configFilePath = configFilePath;
	readParametersFromConfig();
}

//_____________________________________________________________________________
void configReader::readParametersFromConfig() {
	_nChannels = readIntegerFromConfig( _configFilePath, "nChannels");
	_samplingRate = readIntegerFromConfig( _configFilePath, "samplingRate");
	_bufferSize = readIntegerFromConfig( _configFilePath, "bufferSize");
	_refreshRate = readIntegerFromConfig( _configFilePath, "refreshRate");
	_channelsArr = new int[_nChannels];
	readArrayFromConfig( _configFilePath, "channels", ",", _channelsArr);

	_port = readIntegerFromConfig(_configFilePath, "port");
	_ipAddr = readStringFromConfig(_configFilePath, "ip");
}

//_____________________________________________________________________________
int configReader::readIntegerFromConfig(const std::string& cfgFilePath, std::string parameterName) {
	std::ifstream in (cfgFilePath);
	std::string param;
	std::string value;

	int retValue;
	while (!in.eof()) {
		in >> param;
		in >> value;
		if (param == parameterName) {
			retValue = std::stoi(value);
		}
	}
	return retValue;
}

//_____________________________________________________________________________
std::string configReader::readStringFromConfig(const std::string& cfgFilePath, std::string parameterName) {
	std::ifstream in (cfgFilePath);
	std::string param;
	std::string value;
	std::string retValue;
	while (!in.eof()) {
		in >> param;
		in >> value;
		if (param == parameterName) {
			retValue = value;
		}
	}
	return retValue;
}

//_____________________________________________________________________________
void configReader::readArrayFromConfig(const std::string& cfgFilePath, std::string parameterName, std::string delimiter, int* arr) {
	std::ifstream in (cfgFilePath);
	std::string param;
	std::string value;
	while (!in.eof()) {
		in >> param;
		in >> value;
		int cur_idx = 0;
		if (param == parameterName) {
			size_t pos = 0;
			int cur_channel;
			while ((pos = value.find(delimiter)) != std::string::npos) {
				cur_channel = std::stoi(value.substr(0, pos));
				value.erase(0, pos + delimiter.length());
				arr[cur_idx] = cur_channel;
				cur_idx++;
			}
			arr[cur_idx] = std::stoi(value);  // do not forget last value
		}
	}
}
