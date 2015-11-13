#pragma once

#include <unordered_map>
#include <protocol/redis.h>


typedef std::unordered_map<RedisValue> redisHashTable;

class HashTable {
private:

    redisHashTable table;

public:

    HashTable();

    redisHashTable* getTable();

};

