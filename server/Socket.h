#pragma once

class Socket {
private:
    int socketDiscripter_ = 0;
public:
    Socket();
    int getSD();
};
