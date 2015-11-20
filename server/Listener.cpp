#include "Listener.h"


Listener::Listener(int port, int macCli) {
    if (port < 0 || port > 65535) {
        throw std::invalid_argument("bad id of port");
    } else {
        maxCountClient_ = macCli;

        if ((socListenerDisc_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("cannot open listener socket");
        }
        int opt = 1;
        setsockopt(socListenerDisc_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in ba;
        ba.sin_family = AF_INET;
        ba.sin_port = htons(port);
        ba.sin_addr.s_addr = INADDR_ANY;
        if (bind(socListenerDisc_, (struct sockaddr*) &ba, sizeof(ba)) < 0) {
            throw std::runtime_error("cannot bind listener socket");
        }
    }
}
/*
void Listener::listen() {
    if (listen(socListenerDisc_, maxCountClient_) < 0) {
        throw std::runtime_error("cannot listen socket");
    }
}
*/