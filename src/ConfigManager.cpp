#include "PackageManager.hpp"
#include "ConfigManager.hpp"
#include "ProfileManager.hpp"
#include <experimental/filesystem>

ConfigManager *ConfigManager::instance = nullptr;

ConfigManager::ConfigManager()
{
    instance = this;
    userDataDir = ProfileManager::getInstance()->getUserDataDirString();
}

std::string ConfigManager::getRemoteServiceHost()
{
    std::fstream RemoteServerFileStream;
    if (!std::experimental::filesystem::v1::exists(userDataDir+ "RemoteServer"))
    {
        RemoteServerFileStream.open(userDataDir+ "RemoteServer", std::ios_base::out);
        RemoteServerFileStream << "localhost" << std::endl;
        RemoteServerFileStream.close();
        return "localhost";
    } else
    {
        std::__cxx11::string remoteHost;
        RemoteServerFileStream.open(userDataDir+ "RemoteServer", std::ios_base::in);
        RemoteServerFileStream >> remoteHost;
        return remoteHost;
    }
}

ConfigManager *ConfigManager::getInstance()
{
    return instance;
}

ConfigManager::~ConfigManager()
{
    instance = nullptr;
}
