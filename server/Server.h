#pragma once

#include <gtest/gtest.h>

class Server {
public:

    Server();

    void listen();

    void serve(int maxAccept = -1);

    virtual void handle(int conn) = 0;

};

