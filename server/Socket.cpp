#include "Socket.h"

Socket::Socket(int socketDescriptor) {
    this->socketDescriptor_ = socketDescriptor;
}

Socket::~Socket() {
    close(socketDescriptor_);
}

int Socket::getSocketDescriptor() {
    return socketDescriptor_;
}

void Socket::writeData(std::string data) {
    int readSize = 0;
    while (readSize < data.size())
        readSize += write(socketDescriptor_, &readSize, data.size() - readSize);
}

std::string Socket::readData(size_t outSize) {
    std::string outString;
    outString.resize(outSize);
    ssize_t size = read(socketDescriptor_, const_cast<char *>(outString.c_str()), outSize);
    outString.resize(size);
    return outString;
}
