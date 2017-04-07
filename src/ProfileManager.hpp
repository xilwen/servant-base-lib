#ifndef SERVANTBASE_PROFILEMANAGER_HPP
#define SERVANTBASE_PROFILEMANAGER_HPP

#include <experimental/filesystem>
#include <fstream>
#include "VBoxWrapperClient.hpp"

namespace fs = std::experimental::filesystem;


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



private:
    static ProfileManager *instance;
    bool firstTime;
    fs::path userHomeDir;
    fs::path userDataDir;

    void initDataDir();

    void initHomePath(std::__cxx11::wstring &userHomePath);
};


#endif //SERVANTBASE_PROFILEMANAGER_HPP
