#ifndef TESTEXE_BASE_H
#define TESTEXE_BASE_H

#include "ProfileManager.hpp"
#include "FirstTimeUtilities.hpp"
#include "HtmlFileDownloader.hpp"
#include "Logger.hpp"
#include "PackageManager.hpp"
#include "PerformanceManager.hpp"
#include "ProgramHolder.hpp"
#include "VBoxWrapperClient.hpp"
#include "VBoxWrapperHolder.hpp"
#include "VBoxWrapperMessenger.hpp"
#include "VirtualMachine.hpp"
#include "ConfigManager.hpp"
#include "MallManager.hpp"

class ServantBase
{
public:
    static ServantBase *getInstance();

    ~ServantBase();

    void initialWithLocalWrapper();

    VBoxWrapperHolder *getVBoxWrapperHolder() const;

    VBoxWrapperClient *getVBoxWrapperClient() const;

    PackageManager *getPackageManager() const;

    PerformanceManager *getPerformanceManager() const;

    ProfileManager *getProfileManager() const;

    ConfigManager *getConfigManager() const;

    MallManager *getMallManager() const;

    void resetBase();

private:
    ServantBase();

    static ServantBase *instance;
    VBoxWrapperHolder *vBoxWrapperHolder;
    VBoxWrapperClient *vBoxWrapperClient;
    PackageManager *packageManager;
    PerformanceManager *performanceManager;
    ProfileManager *profileManager;
    ConfigManager *configManager;
    MallManager *mallManager;

    bool successFlag = false;


};

#endif //TESTEXE_BASE_H
