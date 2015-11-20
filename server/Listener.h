#pragma once

#include <memory>
#include "Socket.h"

class Listener {
private:
    int socListenerDisc_ = 0;
    int maxCountClient_ = 0;
public:
    Listener(int port = 6376, int maxCli = 10);
    void listen();
    std::unique_ptr<Socket> accept();
};