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
    switch(r->read_char()) {
        case ':': {
            *value = r->read_int();
            break;
        }
        default:
            throw std::runtime_error("invalid redis value");
    }
}
