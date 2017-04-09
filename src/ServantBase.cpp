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
    profileManager = new ProfileManager(WindowsUtilities::getHomePath());
    try
    {
        vBoxWrapperHolder = new VBoxWrapperHolder(
                Utilities::toString(WindowsUtilities::getHomePath()) + "/SERVANT/vBoxWrapper.exe");
    }
    catch (...)
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
    packageManager = new PackageManager;
    configManager = new ConfigManager;
    mallManager = new MallManager;
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
    if (configManager)
        delete configManager;
    if (mallManager)
        delete mallManager;
    if (profileManager)
        delete profileManager;
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

ProfileManager *ServantBase::getProfileManager() const
{
    return profileManager;
}

ConfigManager *ServantBase::getConfigManager() const
{
    return configManager;
}

MallManager *ServantBase::getMallManager() const
{
    return mallManager;
}
