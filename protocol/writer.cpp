#include "writer.h"

void Writer::writeInt(int64_t i) {
    char buff[128];
    int len = snprintf(buff, sizeof(buff), "%ld", i);
    writeRaw(buff, len);
}

void Writer::writeRaw(const char *s, size_t len) {
    for (int i = 0; i < len; ) {
        while (i < len && wpos_ != buffer_.size()) {
            buffer_[wpos_++] = s[i++];
        }
        checkBuffer();
    }
}

void Writer::writeChar(char c) {
    buffer_[wpos_++] = c;
    checkBuffer();
}

void Writer::writeString(const std::string &s) {
    bool prev_is_cr = false;
    size_t i = 0;
    while (
            i < s.size()
            && wpos_ != buffer_.size()
            && !(prev_is_cr && '\n' == s[i])
            ) {
        buffer_[wpos_++] = s[i];
        prev_is_cr = ('\r' == s[i++]);
    }
    checkBuffer();
}


void Writer::writeCrlf() {
    writeChar('\r');
    writeChar('\n');
}

void StringWriter::flush() {
    result.append(buffer_.begin(), buffer_.begin() + wpos_);
    wpos_ = 0;
}

void StringWriter::checkBuffer() {
    if (wpos_ == buffer_.size())
        flush();
}

