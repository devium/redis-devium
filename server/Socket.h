#pragma once

#include <unistd.h>
#include <string>

class Socket {
private:

    int socketDescriptor_;

public:

    Socket(int socketDescriptor);

    ~Socket();

    int getSocketDescriptor();

    std::string readData(size_t size);

    void writeData(std::string data);

};
