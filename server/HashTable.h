#pragma once

#include <unordered_map>
#include <protocol/redis.h>

typedef std::unordered_map<std::string, RedisValue> RedisHashTable;

class HashTable {
private:

    RedisHashTable table_;

public:

    HashTable();

    HashTable(RedisHashTable hashTable);

    RedisHashTable * getTable();

};

