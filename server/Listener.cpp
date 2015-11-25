#include "Listener.h"

Listener::Listener(int port) {
    if (port >= max_port_ && port < min_port_) {
        // todo: deal with errors
    }
    port_ = port;
    socketDescriptor_ = socket(PF_INET, SOCK_STREAM, 0);
    bindSocket();
    setSocketOptions();
}

void Listener::bindSocket() {
    struct sockaddr_in ba;
    ba.sin_family = AF_INET;
    ba.sin_port = htons(port_);
    ba.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketDescriptor_, (struct sockaddr*) &ba, sizeof(ba)) < 0) {
        //todo: deal with errors
    }
}

void Listener::setSocketOptions() {
    int opt = 1;
    setsockopt(socketDescriptor_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in ba;
    ba.sin_family = AF_INET;
    ba.sin_port = htons(port_);
    ba.sin_addr.s_addr = INADDR_ANY;
    if (bind(socketDescriptor_, (struct sockaddr*) &ba, sizeof(ba)) < 0) {
        // todo: deal with errors
    }
}

void Listener::listenSocket(int amountOfConnections) {
    if (listen(socketDescriptor_, amountOfConnections)) {
        // todo: deal with errors
    }
}

std::unique_ptr<Socket> Listener::accept() {
    struct sockaddr_in aa;
    socklen_t slen = sizeof(aa);
    int afd = accept(socketDescriptor_, (struct sockaddr *) &aa, &slen);
    if (afd < 0) {
        // todo: deal with errors
    }
    std::unique_ptr<Socket> p = new Socket(socketDescriptor_);
    return p;

}

Listener::~Listener() {
    close(socketDescriptor_);
}