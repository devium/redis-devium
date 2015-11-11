#pragma once

#include <iosfwd>
#include <protocol/redis.h>

class Cmd {

    virtual ~Cmd() {};
    virtual std::string  name() = 0;
    virtual RedisValue exec(RedisValue args) = 0;

};

