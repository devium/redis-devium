#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    // todo correct 'initialize' error
    if (value.which() == REDIS_INT) {
        w->writeChar(':');
        w->writeInt(boost::get<int64_t>(value));
        w->writeCrlf();
    } else if (value.which() == REDIS_STRING) {
        w->writeChar('+');
        w->writeString(boost::get<std::string>(value));
        w->writeCrlf();
    } else if (value.which() == REDIS_BULK_STRING) {
        w->writeChar('$');
        w->writeInt(boost::get<RedisBulkString>(value).container.size());
        w->writeCrlf();
        w->writeString(boost::get<RedisBulkString>(value).container);
        w->writeCrlf();
    } else if (value.which() == REDIS_ERROR) {
        w->writeChar('-');
        w->writeString(boost::get<RedisError>(value).msg);
        w->writeCrlf();
    } else if (value.which() == REDIS_ARRAY) {
//        w->writeChar('*');
//        w->writeInt(boost::get<std::vector<RedisValue>>(value).size());
//        w->writeCrlf();
//        for (int i = 0; i < boost::get<std::vector<RedisValue>>(value).size(); ++i) {
//            WriteRedisValue(w, &boost::get<std::vector<RedisValue>>(value)[i]);
//        }
    } else if (value.which() == REDIS_NULL) {
        w->writeChar('$');
        w->writeChar('-');
        w->writeChar('1');
        w->writeCrlf();
    } else {
        throw std::runtime_error("unsupported type");
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    switch (r->readChar()) {
        case ':': {
            *value = r->readInt();
            break;
        }
        case '+': {
            *value = r->readLine();
            break;
        }
        case '-': {
            *value = RedisError(r->readLine());
            break;
        }
        case '$': {
            int64_t string_size = r->readInt();
            if (string_size < 0) {
                *value = RedisNull();
            } else {
                *value = r->readRaw(string_size);
            }
            break;
        }
        case '*': {
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
