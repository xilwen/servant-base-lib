#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_TCPV4_PORT "9870"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "asio.hpp"
#include "VBoxWrapperMessenger.hpp"

using asio::ip::tcp;

class VBoxWrapperMessenger;

class VBoxWrapperClient
{
public:
    VBoxWrapperClient();
    ~VBoxWrapperClient();
    void connect();
    void handShake();
    void waitForPortOpen();
    VBoxWrapperMessenger* message();

    static VBoxWrapperClient *getInstance();

protected:

private:
    const std::string className = "VBoxWrapperClient";
    asio::io_service io_service;
    void init();
    void uninit();
    void restart();
    asio::error_code lastError;
    tcp::resolver::iterator *endpoint_iterator;
    tcp::socket* socket = nullptr;
    VBoxWrapperMessenger *messenger = nullptr;

    static VBoxWrapperClient *instance;
};

#endif // CLIENT_H
