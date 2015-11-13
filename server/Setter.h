#pragma once

#include "Cmd.h"

class Setter : public Cmd {
private:

    const std::string commandName_ = "SET";

public:

    Setter(redisHashTable* hashTable);

    std::string getName();

    RedisValue exec(RedisValue args);

};
