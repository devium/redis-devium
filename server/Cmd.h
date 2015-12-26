#pragma once

#include <protocol/redis.h>
#include "HashTable.h"
#include "configs.h"

class Cmd {
protected:

    cmd name_;

    HashTable* redisHashTable_;

public:

    Cmd(cmd name, HashTable * hashTable);

    cmd getName();

};


class Setter : public Cmd {
public:

    Setter(HashTable * hashTable);

    using Cmd::getName;

    RedisValue exec(RedisValue& args);

};


class Getter : public Cmd {
public:

    Getter(HashTable * hashTable);

    using Cmd::getName;

    RedisValue exec(RedisValue& args);

};

