#pragma once

#include <memory>
#include "Socket.h"

class Listener {
public:
    Listener(int port);
    std::unique_ptr<Socket> accept();
};