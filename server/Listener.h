#pragma once

#include <memory>
#include "Socket.h"

class Listener {

    Listener(int port);

    std::unique_ptr<Socket> accept();

};

