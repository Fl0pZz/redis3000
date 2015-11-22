#include "Server.h"


Server::Server(int port, unsigned int maxCli) : listener_(port, maxCli) {}

void ProdServer::serve() {
    std::deque<std::unique_ptr<Socket>> dequeClients;
    RedisValue val;

    dequeClients.push_back(listener_.acceptCli());

    SocketReader dataFromClient(dequeClients[0]->getSD());
    SocketWriter dataToClient(dequeClients[0]->getSD());

    while (true) {
        ReadRedisValue(&dataFromClient, &val);

        //WriteRedisValue(&dataToClient, val);
        //dataToClient.flush();
    }
}

void TestServer::serve() {
    out_ = listener_.acceptCli();
    if (out_ != nullptr) {
        ++countConn_;
    } else {
        std::cerr << "ERORR! Bad socket!\n";
    }
}