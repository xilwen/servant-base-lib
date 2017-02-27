#ifndef SERVANTBASE_PACKAGEMANAGER_H
#define SERVANTBASE_PACKAGEMANAGER_H

#include <experimental/filesystem>
#include <fstream>
#include "VirtualMachine.hpp"
#include "vBoxWrapperClient.hpp"

namespace fs = std::experimental::filesystem;

class packageManager
{
public:
    packageManager() = delete;

    packageManager(std::wstring userHomePath, vBoxWrapperClient *client);

    ~packageManager();

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

    static packageManager *getInstance();

    std::string getRemoteServiceHost();

private:
    fs::path userHomeDir,
            userDataDir;
    std::string machineDataPath;
    std::wfstream machineDataFileStream;
    std::vector<VirtualMachine> machines;
    vBoxWrapperClient *client;
    bool firstTime;
    static packageManager *instance;

    void initHomePath(std::wstring &userHomePath);

    void initDataDir();

    void loadMachinesArray();

    void initMachineData();
};


#endif //TESTEXE_PACKAGEMANAGER_H
