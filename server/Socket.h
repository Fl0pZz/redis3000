#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <stdexcept>
#include <unistd.h>


class Socket {
private:
    int socketDescripter_ = 0;

public:
    Socket(int sD);
    int getSD();
    ~Socket();
};
