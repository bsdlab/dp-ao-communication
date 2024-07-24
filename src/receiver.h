#include <iostream>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <Winsock2.h>
#include <tuple>
#include <algorithm>
#include <EthernetStandAlone.h>

#ifndef RECEIVER_H
#define RECEIVER_H

class Receiver {
public:
    Receiver(const char* ip_addr, int port);
    std::tuple<char*, int> ReceiveMessage();
private:
    int _recvSocket;
    int _sendSocket;

    std::tuple<char*, int> processCommand(char* message);
    void Close();
};

#endif //RECEIVER_H
