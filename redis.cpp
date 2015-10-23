#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    if (value.which() == REDIS_INT) {
        w->write_char(':');
        w->write_int(boost::get<int64_t>(value));
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
            *value = r->read_error();
            break;
        }
        case '$': {
            int64_t string_size = r->read_first_int();
            if (string_size == NULL) {
                *value = RedisNull();
            } else {
                *value = r->read_raw(string_size);
            }
            break;
        }
        case '*': {
            int64_t string_size = r->read_first_int();
            if (string_size == NULL) {
                *value = RedisNull();
            } else {
                *value = std::vector<RedisValue>(string_size);
                for (size_t i = 0; i < string_size; ++i) {
                    ReadRedisValue(r, &value[i]);
                }
            }
            break;
        }
        default:
            throw std::runtime_error("invalid redis value");
    }
}
