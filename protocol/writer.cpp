#include <iostream>
#include "writer.h"


void Writer::write_int(int64_t i) {
    char buff[128];
    int len = snprintf(buff, sizeof(buff), "%ld", i);

    write_raw(buff, len);
}

void Writer::write_raw(const char* s, size_t len) {
    if (len > MAX_LENGHT_ROWSTRING) {
        throw std::invalid_argument("too large raw_string");
    }

    for(int i = 0; i < len; ) {
        while(i < len && wpos_ != buffer_.size()) {
            buffer_[wpos_++] = s[i++];
        }

        if (wpos_ == buffer_.size())
            flush();
    }
}

void Writer::write_char(char c) {
    buffer_[wpos_++] = c;

    if (wpos_ == buffer_.size())
        flush();
}

void Writer::write_crlf() {
    write_char('\r');
    write_char('\n');
}

void Writer::write_string(const std::string &s) {
    if (s.size() > MAX_LENGHT_STRING) {
        throw std::invalid_argument("too large row string");
    }
    write_raw(s.data(), s.size());
}

void StringWriter::flush() {
    result.append(buffer_.begin(), buffer_.begin() + wpos_);
    wpos_ = 0;
}

void SocketWriter::flush() {
    sock.sendData(std::string(buffer_.begin(), buffer_.begin() + wpos_));
    wpos_ = 0;
}