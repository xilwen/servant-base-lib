#include "PackageManager.hpp"
#include "ConfigManager.hpp"
#include "ProfileManager.hpp"
#include <json.hpp>
#include "ProfileManager.hpp"

using json = nlohmann::json;
ConfigManager *ConfigManager::instance = nullptr;

ConfigManager::ConfigManager()
{
    instance = this;
    userDataDir = ProfileManager::getInstance()->getUserDataDirString();
}

std::string ConfigManager::getRemoteServiceHost()
{
    json j(ProfileManager::getInstance()->getConfigJson());
    if(j.find("remoteRepositoryUrl") != j.end())
    {
        return j.find("remoteRepositoryUrl").value();
    } else
    {
        return std::string();
    }
}

void ConfigManager::setRemoteServiceHost(std::string string)
{
    json j(ProfileManager::getInstance()->getConfigJson());
    if(j.find("remoteRepositoryUrl") != j.end())
    {
        j["remoteRepositoryUrl"] = string;
    } else
    {
        j.emplace("remoteRepositoryUrl", string);
    }
    ProfileManager::getInstance()->writeConfigJson(j);
}

std::string ConfigManager::getLocalIPAddress()
{
    json j(ProfileManager::getInstance()->getConfigJson());
    if(j.find("localIPAddress") != j.end())
    {
        return j.find("localIPAddress").value();
    } else
    {
        return std::string();
    }
}

void ConfigManager::setLocalIPAddress(std::string string)
{
    json j(ProfileManager::getInstance()->getConfigJson());
    if(j.find("localIPAddress") != j.end())
    {
        j["localIPAddress"] = string;
    } else
    {
        j.emplace("localIPAddress", string);
    }
    ProfileManager::getInstance()->writeConfigJson(j);
}

ConfigManager *ConfigManager::getInstance()
{
    return instance;
}

ConfigManager::~ConfigManager()
{
    instance = nullptr;
}
