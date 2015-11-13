#include "reader.h"

char Reader::readChar() {
    if (rpos_ == end_) readMore();
    return buffer_[rpos_++];
}

int64_t Reader::readInt() {
    int64_t i = 0;
    bool negative = false;
    char first = readChar(), next;
    if (first == '-') {
        negative = true;
        next = readChar();
    } else {
        next = first;
    }
    do {
        i *= 10;
        i += next - '0';
        next = readChar();
    } while (next != '\r');
    readChar(); // skip '\n'
    return negative ? -i : i;
}

std::string Reader::readLine() {
    //todo Limit size of string (1 Mb)
    std::string str;
    char next = readChar();
    do {
        str.append(&next);
        next = readChar();
    } while (next != '\r');
    readChar();
    return str;
}

std::string Reader::readRaw(size_t len) {
    //todo Limit len (1 << 15)
    std::string bulk_string;
    bulk_string.resize(len);
    for (size_t i = 0; i < len; ++i) {
        bulk_string[i] = readChar();
    }
    readChar();
    readChar();
    return bulk_string;
}

void StringReader::readMore() {
    if (input.empty()) throw std::runtime_error("end of input");
    end_ = 0;
    rpos_ = 0;
    for (; end_ < input.size() && end_ < buffer_.size(); ++end_) {
        buffer_[end_] = input[end_];
    }
    input.erase(input.begin(), input.begin() + end_);
}
