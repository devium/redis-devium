#pragma once

#include <memory>
#include <stdlib.h>
#include <netinet/in.h>
#include <tclDecls.h>
#include <sys/errno.h>
#include "Socket.h"

class Listener {
private:

    int socketDescriptor_;

    int port_;

    const int max_port_ = 1 << 16 - 1;

    const int min_port_ = 0;

    void bindSocket();

    void setSocketOptions();

public:

    Listener(int port);

    std::unique_ptr<Socket> accept();

    void listenSocket(int amountOfConnections);

    ~Listener();

};

