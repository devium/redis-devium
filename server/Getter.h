#pragma once

#include "Cmd.h"

class Getter : public Cmd {
private:

    const std::string COMMAND_NAME_ = "GET";

public:

    Getter(RedisHashTable * hashTable);

    std::string getName();

    RedisValue exec(RedisValue args);

};
