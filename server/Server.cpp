#include "Server.h"


Server::Server(int port, unsigned int maxCli) : listener_(port, maxCli) {}

void ProdServer::serve() {
    Storage table;
    Set set(&table);
    Get get(&table);
    RedisValue val;
    std::unordered_map<std::string, command> commands {
            {"SET", set.name()},
            {"GET", get.name()}
    };

    std::deque<std::unique_ptr<Socket>> dequeClients;

    dequeClients.push_back(listener_.acceptCli());

    SocketReader dataFromClient(dequeClients[0]->getSD());
    SocketWriter dataToClient(dequeClients[0]->getSD());

    while (true) {
        ReadRedisValue(&dataFromClient, &val);

        switch (commands.at(boost::get<RedisBulkString>(boost::get<std::vector<RedisValue>>(val)[0]).data)) {
            case SET : {
                WriteRedisValue(&dataToClient, set.exec(val));
                dataToClient.flush();
                break;
            }
            case GET : {
                WriteRedisValue(&dataToClient, get.exec(val));
                dataToClient.flush();
                break;
            }
        }
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