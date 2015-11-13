#pragma once

#include <vector>
#include "Listener.h"


class Server {
private:
    Listener listener;
public:
    Server();
    Server(int port);
    void server(int max_accept = -1);
    virtual void handle(int conn) = 0;
};

class TestServer : public Server {
    std::vector<int> conns;
    virtual void handle(int conn) { conns.push_back(conn); }
};

class ProdServer : public Server {
    std::vector<int> conns;
    virtual void handle(int conn) { conns.push_back(conn); }
};

