#pragma once

#include <iosfwd>
#include <protocol/redis.h>
#include "HashTable.h"

class Cmd {
public:

    virtual ~Cmd() {};

    virtual std::string  getName() = 0;

    virtual RedisValue exec(RedisValue args) = 0;

};
