#include "Socket.h"


Socket::Socket(int sD) {
    if (sD < 0) {
        throw std::runtime_error("cannot open socket");
    } else {
        socketDescripter_ = sD;
    }
}

int Socket::getSD() {
    return socketDescripter_;
}

Socket::~Socket() {
    close(socketDescripter_);
}