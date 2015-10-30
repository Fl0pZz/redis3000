#include <vector>
#include <string>
#include <stdexcept>

#include <configs.h>

class Writer {
public:
    explicit Writer(size_t buffer_size) : buffer_(buffer_size) {}

    void write_string(const std::string& s);
    void write_raw(const char* s, size_t len);
    void write_int(int64_t i);
    void write_char(char c);
    void write_crlf(); // write "\r\n"

    virtual void flush() = 0;

protected:
    std::vector<char> buffer_;
    int wpos_ = 0;
};

class StringWriter : public Writer {
public:
    explicit StringWriter(size_t buffer_size = 1024) : Writer(buffer_size) {}

    std::string result;

    virtual void flush() override;
};
