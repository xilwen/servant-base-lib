#include "vBoxWrapperClient.hpp"
#include "logger.hpp"

vBoxWrapperClient *vBoxWrapperClient::instance(nullptr);

vBoxWrapperClient::vBoxWrapperClient()
{
    instance = this;
    init();
}

void vBoxWrapperClient::init()
{
    logger::log(className, __func__, InfoLevel::INFO, "SERVANT Base - VBoxWrapper Client");
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", SERVER_TCPV4_PORT);
    endpoint_iterator = new tcp::resolver::iterator;
    *endpoint_iterator = resolver.resolve(query);
}

void vBoxWrapperClient::waitForPortOpen()
{
    //TODO make it detect other than FTP Port
    asio::io_service io_service;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query("localhost", "ftp");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;

    tcp::socket socket(io_service);
    asio::error_code error = asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    asio::streambuf response;
    //TODO find a better way
    asio::read_until(socket, response, "\n", lastError);
}

void vBoxWrapperClient::connect()
{
    socket = new tcp::socket(io_service);
    try
    {
        asio::connect(*socket, *endpoint_iterator, lastError);
    }
    catch (const std::exception &e)
    {
        logger::log(className, __func__, InfoLevel::ERR, e.what());
        delete socket;
        return;
    }
    catch (...)
    {
        logger::log(className, __func__, InfoLevel::ERR, "Something not std::exception was thrown!");
        delete socket;
        return;
    }
    if (lastError)
        return;
    messenger = new vBoxWrapperMessenger(socket);
    logger::log(className, __func__, InfoLevel::INFO, "Connected to server.");
}


void vBoxWrapperClient::handShake()
{
    if (!socket)
        throw new std::runtime_error("No socket, can not handshake.");
    try
    {
        std::wstring wstring(L"ready");
        if (message()->message(wstring).compare(L"ready") == 0)
        {
            logger::log(className, __func__, InfoLevel::INFO, "Hand shook, looks good.");
        } else
        {
            logger::log(className, __func__, InfoLevel::ERR,
                        "Something wrong happened during handshake. Socket terminated.");
            delete socket;
        }
    }
    catch (std::exception &e)
    {
        logger::log(className, __func__, InfoLevel::ERR, e.what());
    }
}

void vBoxWrapperClient::restart()
{
    uninit();
    init();
    while (message() == nullptr)
    {
        connect();
    }
    handShake();
}

void vBoxWrapperClient::uninit()
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
    logger::log(className, __func__, InfoLevel::INFO, "Client has been turned off.");
}


vBoxWrapperClient::~vBoxWrapperClient()
{
    uninit();
    instance = nullptr;
}

vBoxWrapperMessenger *vBoxWrapperClient::message()
{
    return messenger;
}

vBoxWrapperClient *vBoxWrapperClient::getInstance()
{
    return instance;
}
