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
    PackageManager();

    ~PackageManager();

    std::wstring importOVA(std::wstring path);

    bool OVAImportSucceeded();

    bool OVAImportCompleted();

    int OVAImportProgress();

    void refreshMachines();

    void DeleteMachine(VirtualMachine *virtualMachine);

    void DeleteMachine(const std::wstring &nameOrUuid);

    std::vector<VirtualMachine> *getMachines();

    static PackageManager *getInstance();

private:
    fs::path userDataDir;
    std::string machineDataPath;
    std::fstream machineDataFileStream;
    std::vector<VirtualMachine> machines;
    VBoxWrapperClient *vBoxWrapperClient;
    static PackageManager *instance;

    void loadMachinesArray();

    void initMachineData();
};


#endif //TESTEXE_PACKAGEMANAGER_H
