#pragma once

#include <string>
#include "../protocol/redis.h"
#include "Table.h"


class Cmd {
public:
    virtual ~Cmd() {};
    virtual std::string name() = 0;
    virtual RedisValue exec(RedisValue args) = 0;
};


class Set : public Cmd {
private:
    std::string cmdName = "set";
public:
    Set(std::unordered_map<int, int> * table);
    std::string name();
    RedisValue exec(RedisValue args);
};


class Get : public Cmd {
private:
    std::string cmdName = "get";
public:
    Get(std::unordered_map<int, int> * table);
    std::string name();
    RedisValue exec(RedisValue args);
};