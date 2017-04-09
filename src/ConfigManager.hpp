#ifndef SERVANTBASE_CONFIGMANAGER_HPP
#define SERVANTBASE_CONFIGMANAGER_HPP

#include <experimental/filesystem>


class ConfigManager
{
public:
    ConfigManager();

    virtual ~ConfigManager();

    std::string getRemoteServiceHost();

    static ConfigManager *getInstance();

private:
    static ConfigManager *instance;
    std::string userDataDir;

};


#endif //SERVANTBASE_CONFIGMANAGER_HPP
