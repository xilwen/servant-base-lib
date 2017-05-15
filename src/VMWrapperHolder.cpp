#include <thread>
#include "VBoxWrapperClient.hpp"
#include "VMWrapperHolder.hpp"
#include "Logger.hpp"

VMWrapperHolder *VMWrapperHolder::instance(nullptr);

VMWrapperHolder::VMWrapperHolder()
{
    Logger::log("VMWrapperHolder", __func__, InfoLevel::INFO,
                "Now setting up Windows Service things...");
    serviceControlManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CONNECT);
    instance = this;
}

void VMWrapperHolder::openService()
{
    if (service)
    {
        Logger::log(className, __func__, InfoLevel::INFO, "opening service again.");
        CloseHandle(service);
    }
    service = OpenServiceA(serviceControlManager, serviceName.c_str(),
                           SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!service)
    {
        Logger::log(className, __func__, InfoLevel::INFO,
                    "openServiceA failed with " + std::to_string(GetLastError()));
    }
}

VMWrapperHolder::~VMWrapperHolder()
{
    Logger::log(className, __func__, InfoLevel::INFO, className + " destruction started.");
    if (service)
    {
        CloseHandle(service);
    }
    if (serviceControlManager)
    {
        CloseHandle(serviceControlManager);
    }
    Logger::log(className, __func__, InfoLevel::INFO, className + " destruction finished.");
    instance = nullptr;
}

bool VMWrapperHolder::isRunning()
{
    if (!service)
    { return false; }

    queryServiceStatus();
    return (serviceStatusProcess.dwCurrentState != SERVICE_STOPPED &&
            serviceStatusProcess.dwCurrentState != SERVICE_STOP_PENDING);
}

void VMWrapperHolder::start()
{
    if (isRunning())
    {
        Logger::log(className, __func__, InfoLevel::INFO, "service is already running");
        return;
    }
    if (!service)
    {
        Logger::log(className, __func__, InfoLevel::INFO, "no service handle, opening...");
        openService();
    }
    if (!StartServiceA(service, 0, nullptr))
    {
        Logger::log(className, __func__, InfoLevel::INFO,
                    "StartServiceA failed with " + std::to_string(GetLastError()));
    }
    stopwatch = 0;
    while (isStarting())
    {
        if(stopwatch > 20000)
        {
            Logger::log(className, __func__, InfoLevel::INFO,
                        "20000ms used before started, seems to have some problem.... exit the loop.");
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        stopwatch += 50;
    }

}

void VMWrapperHolder::stop()
{
    if (!isRunning() && !isStarting())
    {
        Logger::log(className, __func__, InfoLevel::INFO, "service is not running or starting?");
    }
    if (!service)
    {
        Logger::log(className, __func__, InfoLevel::INFO, "no service handle, opening...");
        openService();
    }
    if (!ControlService(service, SERVICE_CONTROL_STOP, reinterpret_cast<LPSERVICE_STATUS>(&serviceStatusProcess)));
    {
        Logger::log(className, __func__, InfoLevel::INFO,
                    "service can not be stopped with" + std::to_string(GetLastError()));
    }
    stopwatch = 0;
    while (!isStopped())
    {
        if(stopwatch > 20000)
        {
            Logger::log(className, __func__, InfoLevel::INFO,
                        "20000ms used before stopped, seems to have some problem.... exit the loop.");
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        stopwatch += 50;
    }
}

VMWrapperHolder *VMWrapperHolder::getInstance()
{
    if (instance)
        return instance;
    throw std::runtime_error("no instance created.");
}

bool VMWrapperHolder::isStarting()
{
    if (!service)
    { return false; }
    queryServiceStatus();
    return (serviceStatusProcess.dwCurrentState == SERVICE_START_PENDING);
}

void VMWrapperHolder::queryServiceStatus()
{
    DWORD bytesNeeded;
    if (!QueryServiceStatusEx(service, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&serviceStatusProcess),
                              sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded))
    {
        Logger::log(className, __func__, INFO,
                    "QueryServiceStatusEx failed with " + std::__cxx11::to_string(GetLastError()));
    }
}

bool VMWrapperHolder::isStopped()
{
    if (!service)
    { return false; }
    queryServiceStatus();
    return (serviceStatusProcess.dwCurrentState == SERVICE_STOPPED);
}
