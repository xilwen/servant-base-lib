#include "VBoxWrapperClient.hpp"
#include "Logger.hpp"

VBoxWrapperClient *VBoxWrapperClient::instance(nullptr);

VBoxWrapperClient::VBoxWrapperClient()
{
    instance = this;
    init();
}

void VBoxWrapperClient::init()
{
    Logger::log(className, __func__, InfoLevel::INFO, "SERVANT Base - VBoxWrapper Client");
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", SERVER_TCPV4_PORT);
    endpoint_iterator = new tcp::resolver::iterator;
    *endpoint_iterator = resolver.resolve(query);
}

void VBoxWrapperClient::waitForPortOpen(int port)
{
    //TODO make it detect other than FTP Port
    asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query *query = nullptr;
    if (port == 21)
    {
        query = new tcp::resolver::query("localhost", "ftp");
    } else if (port == 80)
    {
        query = new tcp::resolver::query("localhost", "http");
    }
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(*query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    asio::error_code error = asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
        std::vector<wchar_t> buf;
        socket.read_some(asio::buffer(buf), error);
    }
//    asio::streambuf response;
    //TODO find a better way
//    asio::read_until(socket, response, "\n", lastError);

}

void VBoxWrapperClient::connect()
{
    socket = new tcp::socket(io_service);
    try
    {
        asio::connect(*socket, *endpoint_iterator, lastError);
    }
    catch (const std::exception &e)
    {
        Logger::log(className, __func__, InfoLevel::ERR, e.what());
        delete socket;
        return;
    }
    catch (...)
    {
        Logger::log(className, __func__, InfoLevel::ERR, "Something not std::exception was thrown!");
        delete socket;
        return;
    }
    if (lastError)
        return;
    messenger = new VBoxWrapperMessenger(socket);
    Logger::log(className, __func__, InfoLevel::INFO, "Connected to server.");
}


void VBoxWrapperClient::handShake()
{
    if (!socket)
        throw new std::runtime_error("No socket, can not handshake.");
    try
    {
        std::wstring wstring(L"ready");
        if (message()->message(wstring).compare(L"ready") == 0)
        {
            Logger::log(className, __func__, InfoLevel::INFO, "Hand shook, looks good.");
        } else
        {
            Logger::log(className, __func__, InfoLevel::ERR,
                        "Something wrong happened during handshake. Socket terminated.");
            delete socket;
        }
    }
    catch (std::exception &e)
    {
        Logger::log(className, __func__, InfoLevel::ERR, e.what());
    }
}

void VBoxWrapperClient::restart()
{
    uninit();
    init();
    while (message() == nullptr)
    {
        connect();
    }
    handShake();
}

void VBoxWrapperClient::uninit()
{
    if (endpoint_iterator)
        delete endpoint_iterator;
    if (socket)
    {
        socket->shutdown(asio::ip::tcp::socket::shutdown_both, lastError);
        socket->close();
        delete socket;
    }
    if (messenger)
        delete messenger;
    Logger::log(className, __func__, InfoLevel::INFO, "Client has been turned off.");
}


VBoxWrapperClient::~VBoxWrapperClient()
{
    uninit();
    instance = nullptr;
}

VBoxWrapperMessenger *VBoxWrapperClient::message()
{
    return messenger;
}

VBoxWrapperClient *VBoxWrapperClient::getInstance()
{
    return instance;
}
