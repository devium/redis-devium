#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <boost/variant.hpp>
#include "reader.h"
#include "writer.h"

struct RedisError {

    explicit RedisError(const std::string& msg) : msg(msg) {}

    std::string msg;
};

struct RedisNull {};

struct RedisBulkString {

    explicit RedisBulkString(const std::string& str) : container(str) {}

    std::string container;
};

typedef boost::make_recursive_variant<
          RedisNull
        , int64_t
        , std::string
        , RedisBulkString
        , RedisError
        , std::vector<boost::recursive_variant_>
>::type RedisValue;

enum RedisType {
    REDIS_NULL,
    REDIS_INT,
    REDIS_STRING,
    REDIS_BULK_STRING,
    REDIS_ERROR,
    REDIS_ARRAY
};

enum FirstChar {
    INT_CHAR = ':',
    STRING_CHAR = '+',
    BULK_STRING_CHAR = '$',
    ERROR_CHAR = '-',
    ARRAY_CHAR = '*'
};

void WriteRedisValue(Writer* w, const RedisValue& value);

void ReadRedisValue(Reader* r, RedisValue* value);
