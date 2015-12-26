#pragma once

constexpr size_t MAX_STRING_SIZE_ = 1024 * 1024;
constexpr size_t MAX_STRING_LEN_ = 1 << 15;

enum cmd {SET, GET};

enum RedisType {
    REDIS_NULL,
    REDIS_INT,
    REDIS_STRING,
    REDIS_BULK_STRING,
    REDIS_ERROR,
    REDIS_ARRAY
};
