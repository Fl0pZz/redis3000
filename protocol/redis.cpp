#include "redis.h"


void WriteRedisValue(Writer* w, const RedisValue& value) {
    if (value.which() == REDIS_INT) {
        w->write_char(':');
        w->write_int(boost::get<int64_t>(value));
        w->write_crlf();

    } else if (value.which() == REDIS_STRING) {
        try {
            w->write_char('+');
            w->write_string(boost::get<std::string>(value));
            w->write_crlf();
        } catch (std::invalid_argument& e) {
            // TODO: переделать на нормальное логирование.
            std::cerr << "WRITE_STRING: " << e.what() << '\n';
            throw;
        }

    } else if (value.which() == REDIS_ERROR) {
        try {
            w->write_char('-');
            w->write_string(boost::get<RedisError>(value).msg);
            w->write_crlf();
        } catch (std::invalid_argument& e) {
            // TODO: переделать на нормальное логирование.
            std::cerr << "WRITE_ERROR: " << e.what() << '\n';
            throw;
        }

    } else if (value.which() == REDIS_NULL) {
        w->write_char('$');
        w->write_int(static_cast<int64_t>(-1));
        w->write_crlf();

    } else if (value.which() == REDIS_ARRAY) {
        try {
            w->write_char('*');
            if (boost::get<std::vector<RedisValue>>(value).size() > MAX_LENGHT_ARRAY) {
                throw std::invalid_argument("too large array");
            } else {
                w->write_int(boost::get<std::vector<RedisValue>>(value).size());
                w->write_crlf();
                for (auto elem : boost::get<std::vector<RedisValue>>(value)) {
                    WriteRedisValue(w, elem);
                }
            }
        } catch (std::invalid_argument& e) {
            // TODO: переделать на нормальное логирование.
            std::cerr << "WRITE_ARRAY: " << e.what() << '\n';
            throw;
        }

    } else if (value.which() == REDIS_BULK) {
        try {
            w->write_char('$');
            w->write_int(boost::get<RedisBulkString>(value).data.size());
            w->write_crlf();
            w->write_raw(boost::get<RedisBulkString>(value).data.c_str(),
                         boost::get<RedisBulkString>(value).data.size());
            w->write_crlf();
        } catch (std::invalid_argument& e) {
            std::cerr << "WRITE_BULK: " << e.what() << '\n';
            throw;
        }

    } else {
        throw std::runtime_error("unsupported type");
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    switch(r->read_char()) {
        case ':': {
            try {
                *value = r->read_int();
                break;
            } catch (std::invalid_argument& e) {
                // TODO: переделать на нормальное логирование.
                std::cerr << "READ_INT: " << e.what() << '\n';
                throw;
            }
        } case '+': {
            try {
                *value = r->read_line();
                break;
            } catch (std::invalid_argument& e) {
                // TODO: переделать на нормальное логирование.
                std::cerr << "READ_LINE: " << e.what() << '\n';
                throw;
            }
        } case '-': {
            try {
                *value = RedisError(r->read_line());
                break;
            } catch (std::invalid_argument& e) {
                // TODO: переделать на нормальное логирование.
                std::cerr << "READ_ERROR: " << e.what() << '\n';
                throw;
            }
        } case '$': {
            try {
                int64_t len = r->read_int();
                if (len == -1) {
                    *value = RedisNull();
                } else {
                    *value = RedisBulkString(r->read_raw(len));
                }
                break;
            } catch (std::invalid_argument& e) {
                // TODO: переделать на нормальное логирование.
                std::cerr << "READ_RAW or READ_RAWSTR: " << e.what() << '\n';
                throw;
            }
        } case '*': {
            try {
                int64_t len = 0;
                try {
                    len = r->read_int();
                } catch (std::invalid_argument& e) {
                    // TODO: переделать на нормальное логирование.
                    std::cerr << "READ_INT(ARRAY): " << e.what() << '\n';
                    throw;
                }

                *value = std::vector<RedisValue>();
                boost::get<std::vector<RedisValue>>(*value).resize(len);

                for (int64_t i = 0; i < len; ++i) {
                    ReadRedisValue(r, &(boost::get<std::vector<RedisValue>>(*value)[i]));
                }
                break;
            } catch (std::invalid_argument& e) {
                // TODO: переделать на нормальное логирование.
                std::cerr << "READ_ARRAY: " << e.what() << '\n';
                throw;
            }

        } default:
            throw std::runtime_error("invalid redis value");
    }
}
