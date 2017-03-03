#pragma once
#include <string>
#include "asio.hpp"
using asio::ip::tcp;

//TODO solve race conditions
class VBoxWrapperMessenger
{
public:
    VBoxWrapperMessenger() = delete;
    explicit VBoxWrapperMessenger(tcp::socket* socket);
    ~VBoxWrapperMessenger();
    std::wstring message(std::wstring toSend);
    std::wstring machineMessage(std::wstring machineNameOrId, std::wstring toSend);

private:
    void send(std::wstring wstring) const;
    std::wstring receive();
    asio::error_code lastError;
    tcp::socket* socket;
    bool locked;
};

