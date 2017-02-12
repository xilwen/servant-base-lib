#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_TCPV4_PORT "9870"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "asio.hpp"
#include "vBoxWrapperMessenger.hpp"

using asio::ip::tcp;

class vBoxWrapperMessenger;

class vBoxWrapperClient
{
public:
    vBoxWrapperClient();
    ~vBoxWrapperClient();
    void connect();
    void handShake();
    void waitForPortOpen();
    vBoxWrapperMessenger* message();

    static vBoxWrapperClient *getInstance();

protected:

private:
    const std::string className = "vBoxWrapperClient";
    asio::io_service io_service;
    void init();
    void uninit();
    void restart();
    asio::error_code lastError;
    tcp::resolver::iterator *endpoint_iterator;
    tcp::socket* socket = nullptr;
    vBoxWrapperMessenger *messenger = nullptr;

    static vBoxWrapperClient *instance;
};

#endif // CLIENT_H
