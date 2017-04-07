#include "PackageManager.hpp"
#include "ProfileManager.hpp"
#include "Logger.hpp"

ProfileManager *ProfileManager::instance = nullptr;


ProfileManager::ProfileManager(std::wstring userHomePath)
{
    instance = this;
    initHomePath(userHomePath);
    initDataDir();
}

ProfileManager::~ProfileManager()
{
    instance = nullptr;
}

ProfileManager *ProfileManager::getInstance()
{
    return instance;
}

bool ProfileManager::isFirstTime()
{
    return firstTime;
}

void ProfileManager::initDataDir()
{
    userDataDir = userHomeDir.wstring() + std::__cxx11::wstring(L"SERVANTConfig");
    if (std::experimental::filesystem::v1::exists(userDataDir))
    {
        firstTime = false;
        Logger::log("PackageManager", __func__, INFO, "Data directory exist.");

    } else
    {
        firstTime = true;
        std::experimental::filesystem::v1::create_directory(userDataDir);
        Logger::log("PackageManager", __func__, INFO, "Data directory created.");
    }
}

void ProfileManager::initHomePath(std::__cxx11::wstring &userHomePath)
{
    auto backslashPos(userHomePath.find(L'\\'));
    while (backslashPos != std::string::npos)
    {
        userHomePath.replace(backslashPos, 1, L"/");
        backslashPos = userHomePath.find(L'\\');
    }
    if (userHomePath.back() != L'/')
    {
        userHomePath.push_back(L'/');
    }
    userHomeDir = userHomePath;
    Logger::log("PackageManager", __func__, INFO, std::wstring(L"userHomePath is " + userHomePath));
}

std::__cxx11::string ProfileManager::getUserDataDirString()
{
    return userDataDir.string();
}

std::__cxx11::wstring ProfileManager::getUserDataDirWstring()
{
    return userDataDir.wstring();
}

fs::path ProfileManager::getUserDataDir()
{
    return userDataDir;
}
