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
    int getIdleTime();
    void stopIdleTimer();

private:
    void send(std::wstring wstring);
    std::wstring receive();
    void timerRunner();
    void resetTimer();
    asio::error_code lastError;
    tcp::socket* socket;
    bool locked;
    bool runTimer;
    int idleTime = 0;
};

