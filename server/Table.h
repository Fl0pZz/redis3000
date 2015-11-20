#pragma once

#include <unordered_map>
#include "protocol/redis.h"

class Table {
private:
    std::unordered_map<int, int> table_;
public:
    Table();
    std::unordered_map<int, int> * getPointerTable();
};