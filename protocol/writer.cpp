#include "writer.h"

void Writer::write_int(int64_t i) {
    char buff[128];
    int len = snprintf(buff, sizeof(buff), "%ld", i);
    write_raw(buff, len);
}

void Writer::write_raw(const char* s, size_t len) {
    for (int i = 0; i < len; ) {
        while (i < len && wpos_ != buffer_.size()) {
            buffer_[wpos_++] = s[i++];
        }
        checkBuffer();
    }
}

void Writer::write_char(char c) {
    buffer_[wpos_++] = c;
    checkBuffer();
}

void Writer::write_string(const std::string& s) {
    bool prev_is_cr = false;
    for (size_t i = 0; i < s.size(); ) {
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
}


void Writer::write_crlf() {
    write_char('\r');
    write_char('\n');
}

void StringWriter::flush() {
    result.append(buffer_.begin(), buffer_.begin() + wpos_);
    wpos_ = 0;
}

void StringWriter::checkBuffer() {
    if (wpos_ == buffer_.size())
        flush();
}

