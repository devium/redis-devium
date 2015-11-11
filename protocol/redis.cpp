#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    // todo correct 'initialize' error
    if (value.which() == REDIS_INT) {
        w->write_char(':');
        w->write_int(boost::get<int64_t>(value));
        w->write_crlf();
    } else if (value.which() == REDIS_STRING) {
        w->write_char('+');
        w->write_string(boost::get<std::string>(value));
        w->write_crlf();
    } else if (value.which() == REDIS_BULK_STRING) {
        w->write_char('$');
        w->write_int(boost::get<RedisBulkString>(value).container.size());
        w->write_crlf();
        w->write_string(boost::get<RedisBulkString>(value).container);
        w->write_crlf();
    } else if (value.which() == REDIS_ERROR) {
        w->write_char('-');
        w->write_string(boost::get<RedisError>(value).msg);
        w->write_crlf();
    } else if (value.which() == REDIS_ARRAY) {
//        w->write_char('*');
//        w->write_int(boost::get<std::vector<RedisValue>>(value).size());
//        w->write_crlf();
//        for (int i = 0; i < boost::get<std::vector<RedisValue>>(value).size(); ++i) {
//            WriteRedisValue(w, &boost::get<std::vector<RedisValue>>(value)[i]);
//        }
    } else if (value.which() == REDIS_NULL) {
        w->write_char('$');
        w->write_char('-');
        w->write_char('1');
        w->write_crlf();
    } else {
        throw std::runtime_error("unsupported type");
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    switch (r->read_char()) {
        case ':': {
            *value = r->read_int();
            break;
        }
        case '+': {
            *value = r->read_line();
            break;
        }
        case '-': {
            *value = RedisError(r->read_line());
            break;
        }
        case '$': {
            int64_t string_size = r->read_int();
            if (string_size < 0) {
                *value = RedisNull();
            } else {
                *value = r->read_raw(string_size);
            }
            break;
        }
        case '*': {
            //todo Set recursion limit to 8 Mb
            int64_t array_size = r->read_int();
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
