#include <iostream>
#include "Socket.h"

Socket::Socket(int sD) {
    if (sD < 0) {
        throw std::runtime_error("SOCKET: Can't open socket");
    } else {
        socketDescripter_ = sD;
    }
}

int Socket::getSD() {
    return socketDescripter_;
}

std::string * Socket::getData(size_t size) {
    std::string *out = new std::string();
    out->resize(size);
    std::cout << "__________\n";
    size = read(socketDescripter_, const_cast<char *>(out->c_str()), size);
    std::cout << "INput data size: " << size << "\n";
    out->resize(size);
    return out;
}

void Socket::sendData(std::string && input) {
    size_t size = 0;
    while ((size += write(socketDescripter_, input.c_str(), input.size())) < input.size()) {}
}

Socket::~Socket() {
    close(socketDescripter_);
}



LocalSocket::LocalSocket() {
    socket_();
    setOpt_();
}

void LocalSocket::socket_() {
    if ((socDisc_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("cannot open socket");
    }
}

void LocalSocket::setOpt_() {
    int opt = 1;
    setsockopt(socDisc_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void LocalSocket::connect_(int port) {
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (connect(socDisc_ , (struct sockaddr *)&server , sizeof(server)) < 0) {
        throw std::runtime_error("cannot connect to localhost");
    }
}

void LocalSocket::write_(std::string && out) {
    size_t size = 0;
    while ((size += write(socDisc_, out.c_str(), out.size() - size)) < out.size()) {}
}

std::string * LocalSocket::read_(unsigned int size) {
    std::string *out = new std::string();
    out->resize(size);
    ssize_t readSize = 0;
    ssize_t lenght = 0;

    do {
        lenght = read(socDisc_, const_cast<char *>(out->c_str()), size - readSize);
        readSize += lenght;
    }
    while (lenght && readSize < size);
    return out;
}

LocalSocket::~LocalSocket() {
    close(socDisc_);
}