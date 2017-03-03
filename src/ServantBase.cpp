#include <thread>
#include "ServantBase.hpp"
#include "WindowsUtilities.hpp"
#include "Utilities.hpp"

ServantBase *ServantBase::getInstance()
{
    static ServantBase instance;
    return &instance;
}

ServantBase::ServantBase()
{
}

void ServantBase::initialWithLocalWrapper()
{
    Logger::setLogFileDir(WindowsUtilities::getHomePath());
    try
    {
        vBoxWrapperHolder = new VBoxWrapperHolder(
                Utilities::toString(WindowsUtilities::getHomePath()) + "/SERVANT/vBoxWrapper.exe");
    } catch (...)
    {
        resetBase();
        return;
    }
    vBoxWrapperHolder->start();
    vBoxWrapperClient = new VBoxWrapperClient;
    while (vBoxWrapperClient->message() == nullptr)
    {
        vBoxWrapperClient->connect();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    vBoxWrapperClient->handShake();
    packageManager = new PackageManager(WindowsUtilities::getHomePath(), vBoxWrapperClient);
    performanceManager = new PerformanceManager;
    successFlag = true;
}

void ServantBase::resetBase()
{
    if (vBoxWrapperHolder)
        delete vBoxWrapperHolder;
    if (performanceManager)
        delete performanceManager;
    if (packageManager)
        delete packageManager;
    if (vBoxWrapperClient)
        delete vBoxWrapperClient;
    successFlag = false;
}

ServantBase::~ServantBase()
{
    resetBase();
}

VBoxWrapperHolder *ServantBase::getVBoxWrapperHolder() const
{
    return vBoxWrapperHolder;
}

VBoxWrapperClient *ServantBase::getVBoxWrapperClient() const
{
    return vBoxWrapperClient;
}

PackageManager *ServantBase::getPackageManager() const
{
    return packageManager;
}

PerformanceManager *ServantBase::getPerformanceManager() const
{
    return performanceManager;
}
