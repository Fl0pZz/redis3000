#include <iostream>
#include "reader.h"


char Reader::read_char() {
    if (rpos_ == end_) read_more();
    return buffer_[rpos_++];
}

int64_t Reader::read_int() {
    int64_t i = 0;
    char ch = 0;
    bool negative = false;

    char first = read_char(), next;
    if (first == '-') {
        negative = true;
        next = read_char();
    } else {
        next = first;
    }

    do {
        i *= 10;
        i += next - '0';

        next = read_char();
        ++ch;
    } while(next != '\r' && ch <= 21);

    if (ch > 19) {
        throw std::invalid_argument("not integer");
    }
    read_char(); // skip '\n'

    return negative ? -i : i;
}

std::string Reader::read_line() {
    std::string out;
    char tmp = read_char();
    ssize_t lenght = 0;

    while (tmp != '\r' && lenght++ <= MAX_LENGHT_STRING) {
        out.push_back(tmp);
        tmp = read_char();
    }

    if (lenght > MAX_LENGHT_STRING) {
        throw std::invalid_argument("too large raw_string");
    }
    read_char(); // skip '\n'
    return out;
}

std::string Reader::read_raw(size_t len) {
    if (len > MAX_LENGHT_ARRAY) {
        throw std::invalid_argument("too large raw_string");
    }

    std::string out;
    out.resize(len);

    for (size_t i = 0; i < len; ++i) {
        out[i] = read_char();
    }
    read_char(); // skip '\r'
    read_char(); // skip '\n'

    return out;
}

void StringReader::read_more() {
    if (input.empty()) throw std::runtime_error("end of input");

    end_ = 0;
    rpos_ = 0;
    for (; end_ < input.size() && end_ < buffer_.size(); ++end_) {
        buffer_[end_] = input[end_];
    }

    input.erase(input.begin(), input.begin() + end_);
}
