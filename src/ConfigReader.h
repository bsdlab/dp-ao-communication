#include <sstream>
#include <fstream>

#ifndef CONFIGREADER_H
#define CONFIGREADER_H

class configReader {
public:
    std::string _configFilePath;
    // Stream parameters
    int _nChannels, _samplingRate, _bufferSize, _refreshRate;
    int* _channelsArr;
    int _port;
    std::string _ipAddr;

    configReader(std::string configFilePath);

private:
    void readParametersFromConfig();
    int readIntegerFromConfig(const std::string& cfgFilePath, std::string parameterName);
    std::string readStringFromConfig(const std::string& cfgFilePath, std::string parameterName);
    void readArrayFromConfig(const std::string& cfgFilePath, std::string parameterName, std::string delimiter, int* arr);

};

#endif //CONFIGREADER_H
