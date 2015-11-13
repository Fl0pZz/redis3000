#pragma once

#include <unordered_map>
#include "protocol/redis.h"

class Table {
private:
    std::unordered_map<RedisValue> table_;
public:
    Table();
    std::unordered_map<RedisValue> * getPointerTable();
};