#include "VBoxWrapperMessenger.hpp"
#include "Logger.hpp"
#include "VMWrapperHolder.hpp"
#include <thread>

VBoxWrapperMessenger::VBoxWrapperMessenger(tcp::socket *socket) : socket(socket)
{
    locked = false;
    runTimer = true;
    std::thread(&VBoxWrapperMessenger::timerRunner, this).detach();
}

VBoxWrapperMessenger::~VBoxWrapperMessenger()
{
    runTimer = false;
}

void VBoxWrapperMessenger::send(std::wstring wstring)
{
    if (!socket)
    {
        throw std::runtime_error("Send Response failed because there is no socket.");
    }
    try
    {
        asio::write(*socket, asio::buffer(wstring));
    }catch(const std::exception& e)
    {
        Logger::log("VBoxWrapperMessenger", __func__, InfoLevel::ERR, wstring);
        Logger::log("VBoxWrapperMessenger", __func__, InfoLevel::ERR, e.what());
    }
    resetTimer();
}

std::wstring VBoxWrapperMessenger::receive()
{
    if (!socket)
    {
        throw std::runtime_error("Receive command failed because there is no socket.");
    }
    std::vector<wchar_t> buf;
    std::wstringstream wstringstream;
    buf.resize(2048);
    auto len = socket->read_some(asio::buffer(buf), lastError);
    for (auto it = buf.begin(); it != (buf.begin() + len); ++it)
    {
        wstringstream << *it;
    }
    resetTimer();
    auto received(wstringstream.str());
    return received.substr(0, received.find(L'\0'));
}

std::wstring VBoxWrapperMessenger::message(std::wstring toSend)
{
    while (locked)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    locked = true;

    send(toSend);
    auto returnValue = receive();

    locked = false;
    return returnValue;
}

std::wstring VBoxWrapperMessenger::machineMessage(std::wstring machineNameOrId, std::wstring toSend)
{
    while (locked)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    locked = true;

    send(L"select search " + machineNameOrId);
    auto machineSelectionResult = receive();
    if (!machineSelectionResult.find(L"SUCCESS"))
    {
        locked = false;
        return machineSelectionResult;
    }

    send(toSend);
    auto returnValue = receive();

    locked = false;
    return returnValue;
}

void VBoxWrapperMessenger::timerRunner()
{
    while (runTimer)
    {
        try
        {
            if (!VMWrapperHolder::getInstance() || !VMWrapperHolder::getInstance()->isRunning())
            {
                break;
            }
        }
        catch(const std::runtime_error &runtimeError)
        {
            Logger::log("VBoxWrapperMessenger", __func__, InfoLevel::ERR, "No VMWrapperHolder is running?");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ++idleTime;
        if (idleTime > 5 )
        {
            message(L"keepAlive");
            resetTimer();
        }
    }
}

void VBoxWrapperMessenger::resetTimer()
{
    idleTime = 0;
}

int VBoxWrapperMessenger::getIdleTime()
{
    return idleTime;
}

void VBoxWrapperMessenger::stopIdleTimer()
{
    runTimer = false;
}
