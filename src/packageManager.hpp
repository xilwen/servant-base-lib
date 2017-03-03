#ifndef SERVANTBASE_PACKAGEMANAGER_H
#define SERVANTBASE_PACKAGEMANAGER_H

#include <experimental/filesystem>
#include <fstream>
#include "VirtualMachine.hpp"
#include "VBoxWrapperClient.hpp"

namespace fs = std::experimental::filesystem;

class PackageManager
{
public:
    PackageManager() = delete;

    PackageManager(std::wstring userHomePath, VBoxWrapperClient *client);

    ~PackageManager();

    std::wstring importOVA(std::wstring path);

    bool OVAImportSucceeded();

    bool OVAImportCompleted();

    int OVAImportProgress();

    void DeleteMachine(VirtualMachine *virtualMachine);

    void DeleteMachine(const std::wstring &nameOrUuid);

    std::vector<VirtualMachine> *getMachines();

    std::string getUserDataDir();

    std::wstring getUserDataDirWstring();

    bool isFirstTime();

    static PackageManager *getInstance();

    std::string getRemoteServiceHost();

private:
    fs::path userHomeDir,
            userDataDir;
    std::string machineDataPath;
    std::wfstream machineDataFileStream;
    std::vector<VirtualMachine> machines;
    VBoxWrapperClient *client;
    bool firstTime;
    static PackageManager *instance;

    void initHomePath(std::wstring &userHomePath);

    void initDataDir();

    void loadMachinesArray();

    void initMachineData();
};


#endif //TESTEXE_PACKAGEMANAGER_H
