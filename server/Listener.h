#pragma once

#include <memory>
#include "Socket.h"

class Listener {
private:
    int socListenerDisc_ = 0;
public:
    Listener(int port);
    std::unique_ptr<Socket> accept();
};