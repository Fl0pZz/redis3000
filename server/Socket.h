#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>


class Socket {
private:
    int socketDescripter_ = 0;
public:
    struct sockaddr_in options_;
    Socket(int sD);
    int getSD();
    void sendData(std::string && input);
    std::string * getData(size_t size);
    ~Socket();
};

class LocalSocket {
public:
    int socDisc_ = -1;
    std::string * out;
    LocalSocket();
    void socket_();
    void setOpt_();
    void connect_(int port);
    void write_(std::string && out);
    std::string * read_(unsigned int size);
    ~LocalSocket();
};
