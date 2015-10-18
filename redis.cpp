#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    if (value.which() == REDIS_INT) {
        w->write_char(':');
        w->write_int(boost::get<int64_t>(value));
        w->write_crlf();

    } else if (value.which() == REDIS_STRING) {
        w->write_char('+');
        w->write_string(boost::get<std::string>(value));
        w->write_crlf();

    } else if (value.which() == REDIS_ERROR) {
        w->write_char('-');
        w->write_string(boost::get<RedisError>(value).msg);
        w->write_crlf();

    } else if (value.which() == REDIS_NULL) {
        w->write_char('$');
        w->write_int(static_cast<int64_t>(-1)); //TODO: удостовериться что это верно!
        w->write_crlf();

    } else if (value.which() == REDIS_ARRAY) {
        w->write_char('*');
        w->write_int(boost::get<std::vector<RedisValue>>(value).size());
        w->write_crlf();
        for (auto elem : boost::get<std::vector<RedisValue>>(value)) {
            WriteRedisValue(w, elem);
        }

    } else {
        //TODO: разобраться с типо bulk string из RESP
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
