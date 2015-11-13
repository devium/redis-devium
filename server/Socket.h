#pragma once

class Socket {
private:

    int socketDescriptor_;

public:

    Socket();

    int getSocketDescriptor();

};
