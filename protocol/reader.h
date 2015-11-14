#pragma once

#include <vector>
#include <string>
#include <stdexcept>

class Reader {
public:

    explicit Reader(size_t bufferSize) : buffer_(bufferSize) {}

    char readChar();

    std::string readLine();

    std::string readRaw(size_t len);

    int64_t readInt();

protected:

    const size_t MAX_STRING_SIZE_ = 1024 * 1024;

    const size_t MAX_STRING_LEN_ = 1 << 15;

    std::vector<char> buffer_;

    size_t end_ = 0, rpos_ = 0;

    virtual void readMore() = 0;

};

class StringReader : public Reader {
public:

    explicit StringReader(size_t bufferSize = 1024) : Reader(bufferSize) {}

    std::string input;

    virtual void readMore() override;

};
