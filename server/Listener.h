#pragma once

#include <memory>
#include "Socket.h"

class Listener {
private:

    int socketDescriptor_;

public:

    Listener(int port);

    std::unique_ptr<Socket> accept();

};

