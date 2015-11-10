#pragma once

#include <string>
#include "protocol/redis.h"


class Cmd {
public:
    virtual ~ cmd() {}
    virtual std::string name() = 0;
    virtual RedisValue exec(RedisValue args) = 0;
};


class Set : public Cmd {
    Set();
    std::string name();
    RedisValue exec(RedisValue args);
};


class Get : public Cmd {
    Get();
    std::string name();
    RedisValue exec(RedisValue args);
};