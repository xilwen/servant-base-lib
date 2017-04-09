#ifndef SERVANTBASE_PROFILEMANAGER_HPP
#define SERVANTBASE_PROFILEMANAGER_HPP

#include <experimental/filesystem>
#include <fstream>
#include <json.hpp>
#include "VBoxWrapperClient.hpp"

namespace fs = std::experimental::filesystem;
using json = nlohmann::json;


class ProfileManager
{
public:
    ProfileManager() = delete;

    ProfileManager(std::wstring userHomePath);

    virtual ~ProfileManager();

    static ProfileManager *getInstance();

    bool isFirstTime();

    fs::path getUserDataDir();

    std::string getUserDataDirString();

    std::wstring getUserDataDirWstring();

    std::string getDownloadDir();

    json getConfigJson();

    void writeConfigJson(const json &input);

    json getMallRepositoryJson();

    void writeMallRepositoryJson(const json &input);

    json getPerformanceJson();

    void writePerformanceJson(const json &input);

private:
    static ProfileManager *instance;
    bool firstTime;
    fs::path userHomeDir;
    fs::path userDataDir;
    fs::path configFilePath;
    fs::path mallRepositoryPath;
    fs::path performanceFilePath;
    fs::path downloadDir;

    void initDataDir();

    void initHomePath(std::__cxx11::wstring &userHomePath);

    void initJsonFilePath();

    json getJsonFromFile(fs::path &filePath) const;

    json createJsonToFile(json j, fs::path &filePath) const;
};


#endif //SERVANTBASE_PROFILEMANAGER_HPP
