#include "PackageManager.hpp"
#include "ProfileManager.hpp"
#include "Logger.hpp"

ProfileManager *ProfileManager::instance = nullptr;


ProfileManager::ProfileManager(std::wstring userHomePath)
{
    instance = this;
    initHomePath(userHomePath);
    initDataDir();
    initJsonFilePath();
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
    userDataDir = userHomeDir.wstring() + std::wstring(L"SERVANTConfig");
    if (std::experimental::filesystem::v1::exists(userDataDir))
    {
        firstTime = false;
        Logger::log("ProfileManager", __func__, INFO, "Data directory exist.");

    } else
    {
        firstTime = true;
        std::experimental::filesystem::v1::create_directory(userDataDir);
        Logger::log("ProfileManager", __func__, INFO, "Data directory created.");
    }
}

void ProfileManager::initHomePath(std::wstring &userHomePath)
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
    Logger::log("ProfileManager", __func__, INFO, std::wstring(L"userHomePath is " + userHomePath));
}

std::string ProfileManager::getUserDataDirString()
{
    return userDataDir.string();
}

std::wstring ProfileManager::getUserDataDirWstring()
{
    return userDataDir.wstring();
}

std::string ProfileManager::getDownloadDir()
{
    return downloadDir.string();
}

fs::path ProfileManager::getUserDataDir()
{
    return userDataDir;
}

json ProfileManager::getConfigJson()
{
    json j;
    if (fs::exists(configFilePath))
    {
        return getJsonFromFile(configFilePath);
    } else
    {
        j = createJsonToFile(
                R"({"remoteRepositoryUrl" : "http://127.0.0.1/SERVANT/repo/", "vBoxWrapperType" : "Local"})"_json,
                configFilePath);
    }
    return j;
}

void ProfileManager::writeConfigJson(const json &input)
{
    createJsonToFile(input, configFilePath);
}

json ProfileManager::getMallRepositoryJson()
{
    json j;
    if (fs::exists(mallRepositoryPath))
    {
        return getJsonFromFile(mallRepositoryPath);
    } else
    {
        j = createJsonToFile(json(), configFilePath);
    }
    return j;
}

void ProfileManager::writeMallRepositoryJson(const json &input)
{
    createJsonToFile(input, mallRepositoryPath);
}

void ProfileManager::initJsonFilePath()
{
    configFilePath = userDataDir.string() + "config.json";
    mallRepositoryPath = userDataDir.string() + "mallRepository.json";
    performanceFilePath = userDataDir.string() + "performance.json";
    downloadDir = userDataDir.string() + "download/";
}

json ProfileManager::getPerformanceJson()
{
    json j;
    if (fs::exists(performanceFilePath))
    {
        return getJsonFromFile(performanceFilePath);
    } else
    {
        j = createJsonToFile(json(), performanceFilePath);
    }
    return j;
}

void ProfileManager::writePerformanceJson(const json &input)
{
    createJsonToFile(input, performanceFilePath);
}

json ProfileManager::createJsonToFile(json j, fs::path &filePath) const
{
    std::ofstream configFile;
    configFile.open(configFilePath.string(), std::ios_base::out | std::ios_base::trunc);
    if (configFile.is_open())
    {
        j >> configFile;
        configFile.close();
        Logger::log("ProfileManager", __func__, INFO, "Created " + configFilePath.string());
    } else
    {
        Logger::log("ProfileManager", __func__, INFO, "Error creating " + configFilePath.string());
    }
    return j;
}

json ProfileManager::getJsonFromFile(fs::path &filePath) const
{
    json j;
    std::ifstream file;
    file.open(filePath.string(), std::ios_base::in);
    if (file.is_open())
    {
        file >> j;
        file.close();
        Logger::log("ProfileManager", __func__, INFO, "Read " + filePath.string());
    } else
    {
        Logger::log("ProfileManager", __func__, INFO, "Error reading " + filePath.string());
    }
    return j;
}

