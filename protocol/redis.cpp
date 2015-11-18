#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    if (value.which() == REDIS_INT) {
        w->writeChar(INT_CHAR);
        w->writeInt(boost::get<int64_t>(value));
        w->writeCrlf();
    } else if (value.which() == REDIS_STRING) {
        w->writeChar(STRING_CHAR);
        w->writeString(boost::get<std::string>(value));
        w->writeCrlf();
    } else if (value.which() == REDIS_BULK_STRING) {
        w->writeChar(BULK_STRING_CHAR);
        w->writeInt(boost::get<RedisBulkString>(value).container.size());
        w->writeCrlf();
        w->writeRaw(boost::get<RedisBulkString>(value).container.c_str(), boost::get<RedisBulkString>(value).container.size());
        w->writeCrlf();
    } else if (value.which() == REDIS_ERROR) {
        w->writeChar(ERROR_CHAR);
        w->writeString(boost::get<RedisError>(value).msg);
        w->writeCrlf();
    } else if (value.which() == REDIS_ARRAY) {
        w->writeChar(ARRAY_CHAR);
        auto& vec = boost::get<std::vector<RedisValue>>(value);
        w->writeInt(vec.size());
        w->writeCrlf();
        for (int i = 0; i < vec.size(); ++i) {
            WriteRedisValue(w, vec[i]);
        }
    } else if (value.which() == REDIS_NULL) {
        w->writeChar(BULK_STRING_CHAR);
        w->writeInt(-1);
        w->writeCrlf();
    } else {
        throw std::runtime_error("unsupported type");
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    switch (r->readChar()) {
        case INT_CHAR: {
            *value = r->readInt();
            break;
        }
        case STRING_CHAR: {
            *value = r->readLine();
            break;
        }
        case ERROR_CHAR: {
            *value = RedisError(r->readLine());
            break;
        }
        case BULK_STRING_CHAR: {
            int64_t string_size = r->readInt();
            if (string_size < 0) {
                *value = RedisNull();
            } else {
                *value = r->readRaw(string_size);
            }
            break;
        }
        case ARRAY_CHAR: {
            //todo Set recursion limit to 8 Mb
            int64_t array_size = r->readInt();
            *value = std::vector<RedisValue>();
            boost::get<std::vector<RedisValue>>(*value).resize(array_size);
            for (size_t i = 0; i < array_size; ++i) {
                ReadRedisValue(r, &(boost::get<std::vector<RedisValue>>(*value)[i]));
            }
            break;
        }
        default:
            throw std::runtime_error("invalid redis value");
    }
}
