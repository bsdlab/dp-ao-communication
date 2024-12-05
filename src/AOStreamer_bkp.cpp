#include "ConfigReader.h"
#include "helpers.h"
#include "receiver.h"
#include <Python.h>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <deque>
#include <iostream>
#include <lsl_cpp.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

// #include "Window.h"

class AlphaOmegaAPI {
  const std::string _configFilePath = "./config/AO_STREAM.CONFIG";
  configReader CR = configReader(_configFilePath);

  // Stream parameters
  const int _nChannels = CR._nChannels;
  const int _samplingRate = CR._samplingRate;
  const int _bufferSize = CR._bufferSize;
  const int _refreshRate = CR._refreshRate;
  int *_channelsArr = CR._channelsArr;

  // Network parameters
  const char *_ipAddr = CR._ipAddr.c_str();
  const int _port = CR._port;

  // LSL streams
  // stream_outlet class comes without default constructor :/
  // Data stream
  lsl::stream_outlet *_dataLSLOutlet;

  // Function call stream
  lsl::stream_outlet *_functionLSLOutlet;

  // Declare class for receiving functions via tcp
  Receiver _messageReceiver = Receiver(_ipAddr, _port);

  // Buffer state queue: Contains boolean values for each run (buffer was empty
  // in that run)
  std::deque<bool> _bufferStateQueue;
  int _n = 10; // Times of which the buffer can be filled with zeros without
               // throwing warning

  // Pop-Up window for function call error
  // Window* _errorWindow = new Window();

public:
  AlphaOmegaAPI() {
    printf("Creating LSL outlet");

    lsl::stream_info _functionCallOutletInfo =
        lsl::stream_info("AOFctCallStream", "String", 1, lsl::IRREGULAR_RATE,
                         lsl::cf_string, "id0");
    lsl::stream_info _dataOutletInfo =
        lsl::stream_info("AODataStream", "EEG", _nChannels, _samplingRate,
                         lsl::cf_float32, "id1");
    _dataLSLOutlet = new lsl::stream_outlet(_dataOutletInfo, 1, _bufferSize);
    _functionLSLOutlet = new lsl::stream_outlet(_functionCallOutletInfo, 1, 1);

    // Connect with AO and add buffer for all channels
    printf("Connecting to AO");
    ao_connect();

    printf("Setting up buffers");
    bufferChannels();
  }

  void run() {
    // Data parameters
    ULONG TS_Begin = 0;
    int16 *buffer = new int16[_bufferSize];
    int actualData = 0;
    int funcRet;

    std::cout << "Start streaming" << std::endl;
    int run = 0;
    while (true) {
      // Sleep(_refreshRate);
      // Only query after sleeping for _refreshRate milliseconds to no overload
      // the CPU
      std::this_thread::sleep_for(std::chrono::milliseconds(_refreshRate));

      // Fill buffer with data from AO
      funcRet = GetAlignedData(buffer, _bufferSize, &actualData, _channelsArr,
                               _nChannels, &TS_Begin);

      // Stream data from buffer and function calls
      singleRun(buffer, actualData);
      run++;
    }
    ao_closeConnection();
  }

  void run_from_txt_file(std::string filename) {
    int actualData;
    int nBlocks = getNumberOfLines(filename);
    int *actualDataArr = new int[nBlocks];
    int16 **buffer = new int16 *[nBlocks];
    for (int i = 0; i < nBlocks; i++) {
      buffer[i] = new int16[_bufferSize];
    }

    // Fills buffer from .txt file
    readMockUpAlignedData(buffer, filename);
    getActualDataNumber(actualDataArr, buffer, _bufferSize, nBlocks);

    // Stream data
    std::cout << "Start streaming" << std::endl;
    int run = 0;
    while (run < nBlocks) {
      std::cout << "start run " << run << std::endl;
      actualData = actualDataArr[run];
      singleRun(buffer[run], actualData);
      run++;
    }
  }

private:
  void singleRun(int16 *buffer, int actualData) {
    std::tuple<char *, int> msgErrorCodeTuple;
    std::string message;
    int errorCode;

    // Check if buffer was empty for _n runs
    checkBufferState(buffer);
    // Get number of data per channel
    int nChannelData = actualData / _nChannels;

    // Slice data and bring it into the following shape: (n_times, n_chs)
    std::vector<std::vector<float>> slicedData =
        sliceData(buffer, actualData, _nChannels);

    //  Push each sample and simultaneously read and execute incomming commands
    //  on AO via tcp connection
    for (int j = 0; j < nChannelData; j++) {
      // sleep(_refreshRate);
      // Push one sample of each channel, i.e. vector of shape (nChannels, 1)
      _dataLSLOutlet->push_sample(slicedData.at(j));

      // // Reveice and execute function call on AO
      msgErrorCodeTuple = _messageReceiver.ReceiveMessage();
      message = std::get<0>(msgErrorCodeTuple);
      errorCode = std::get<1>(msgErrorCodeTuple);

      if (errorCode == 0 && !message.empty()) {
        // Push function call
        _functionLSLOutlet->push_sample(std::vector<std::string>{message});
        std::cout << "Function call was successful :)" << std::endl;
      } else if (errorCode != 0 && !message.empty()) {
        std::cout << "Function call was not successful :(" << std::endl;
        std::cout << "Error code: " << errorCode << std::endl;
      }

      // Pop-up window if an error occurs
      // _errorWindow->ProcessMessages(errorCode);
    }
  }

  void checkBufferState(int16 *buffer) {
    bool emptyBuffer = checkBufferIsEmpty(buffer);
    _bufferStateQueue.push_back(emptyBuffer);
    bool bufferEmptySinceNRuns = checkBufferIsEmptyForNRuns();

    if (bufferEmptySinceNRuns) {
      std::cout << "Buffer was empty for " << _n << "runs" << std::endl;
    }
  }
  static bool checkBufferIsEmpty(int16 *buffer) {
    bool isEmpty = true;
    for (int i = 0; i < sizeof(buffer); i++) {
      if (buffer[i] != 0) {
        isEmpty = false;
      }
    }
    return isEmpty;
  }

  bool checkBufferIsEmptyForNRuns() {
    bool bufferIsEmptyForNRuns = true;
    if (_bufferStateQueue.size() >= _n) {
      // Iterate over queue
      for (auto &curEl : _bufferStateQueue) {
        // set var to false if one element is false
        if (!curEl) {
          bufferIsEmptyForNRuns = false;
        }
      }
      // Remove the first inserted element
      _bufferStateQueue.pop_front();
      return bufferIsEmptyForNRuns;
      // If size of queue is not _n, we do not check and return false
    } else {
      return false;
    }
  }

  void bufferChannels() {
    std::cout << "Buffering channels..." << std::endl;
    for (int i = 0; i < _nChannels; i++) {
      AddBufferChannel(_channelsArr[i], _bufferSize);
    }
  }

  static void ao_connect() {
    int mac_adress_iter[6] = {0x6c, 0xec, 0xeb, 0xff, 0xeb, 0x18};
    int next;

    MAC_ADDR dsp;
    int i = 0;
    for (int i = 0; i < 6; i++) {
      next = mac_adress_iter[i];
      dsp.addr[i] = next;
    }
    std::cout << "connecting to AO - DSP MAC address -> ";
    for (size_t i = 0; i < 6; i++) {
      std::cout << std::hex << dsp.addr[i] << ':';
    }
    std::cout << std::endl;

    int connect;
    connect = DefaultStartConnection(&dsp, 0);
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura);
    std::cout << "isconnected ->" << isConnected() << std::endl;

    if (isConnected() == FALSE) {
      std::cout << "connection failed" << std::endl;
    } else {
      std::cout << "connection successful " << std::endl;
    }
  }

  static void ao_closeConnection() {
    int ret;
    ret = CloseConnection();
  }

  // This function takes int16* data -> this is for executing on AO
  static std::vector<std::vector<float>> sliceData(int16 *data, int actualData,
                                                   int n_channels) {
    // Get number of samples per channel
    int nChannelData = actualData / n_channels;

    // Init vector of shape (nChannelData, nChannels)
    std::vector<std::vector<float>> resData(nChannelData,
                                            std::vector<float>(n_channels, 0));

    int curActualData = 0;
    for (int i = 0; i < n_channels; i++) {
      for (int j = 0; j < nChannelData; j++) {
        resData[j][i] = data[curActualData];
        curActualData += 1;
      }
    }
    return resData;
  }
};

int main_bkp(int argc, char *argv[]) {
  std::string configFilePath = "./config/AO_STREAM.CONFIG";
  configReader CR = configReader(configFilePath);

  // Network parameters
  const char *_ipAddr = CR._ipAddr.c_str();
  const int _port = CR._port;

  printf("Creating server at ip=%s:%d \n", _ipAddr, _port);

  AlphaOmegaAPI AOAPI = AlphaOmegaAPI();
  printf("AOApi initialized");

  // AOAPI.run()
  AOAPI.run();
  return 0;
}
