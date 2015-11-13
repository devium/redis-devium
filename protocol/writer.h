#pragma once

#include <vector>
#include <string>

class Writer {
public:

    explicit Writer(size_t bufferSize) : buffer_(bufferSize) {}

    void writeString(const std::string &s);

    void writeRaw(const char *s, size_t len);

    void writeInt(int64_t i);

    void writeChar(char c);

    void writeCrlf();

    virtual void flush() = 0;

    virtual void checkBuffer() = 0;

protected:

    std::vector<char> buffer_;

    int wpos_ = 0;

};

class StringWriter : public Writer {
public:

    explicit StringWriter(size_t bufferSize = 1024) : Writer(bufferSize) {}

    std::string result;

    virtual void flush() override;

    virtual void checkBuffer() override;

};
