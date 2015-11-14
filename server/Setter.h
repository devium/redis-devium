#pragma once

#include "Cmd.h"

class Setter : public Cmd {
private:

    const std::string COMMAND_NAME_ = "SET";

public:

    Setter(redisHashTable* hashTable);

    std::string getName();

    RedisValue exec(RedisValue args);

};
