#include "VBoxWrapperMessenger.hpp"
#include <thread>

VBoxWrapperMessenger::VBoxWrapperMessenger(tcp::socket* socket):socket(socket)
{
    locked = false;
}

VBoxWrapperMessenger::~VBoxWrapperMessenger()
{
}

void VBoxWrapperMessenger::send(std::wstring wstring) const
{
    if (!socket)
    {
        throw std::runtime_error("Send Response failed because there is no socket.");
    }
    asio::write(*socket, asio::buffer(wstring));
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
    auto received(wstringstream.str());
    return received.substr(0, received.find(L'\0'));
}

std::wstring VBoxWrapperMessenger::message(std::wstring toSend)
{
    while(locked)
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
    while(locked)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    locked = true;

    send(L"select search "+ machineNameOrId);
    auto machineSelectionResult = receive();
    if(!machineSelectionResult.find(L"SUCCESS"))
    {
        locked = false;
        return machineSelectionResult;
    }

    send(toSend);
    auto returnValue = receive();

    locked = false;
    return returnValue;
}
