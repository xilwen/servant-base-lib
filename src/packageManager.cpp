#include "PackageManager.hpp"
#include "Logger.hpp"

namespace fs = std::experimental::filesystem;

PackageManager *PackageManager::instance(nullptr);

PackageManager::PackageManager(std::wstring userHomePath, VBoxWrapperClient *client) : client(client)
{
    initHomePath(userHomePath);
    initDataDir();
    instance = this;
    initMachineData();
}

void PackageManager::initMachineData()
{
    std::__cxx11::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    machineDataPath = converter.to_bytes(userDataDir.wstring() + L"/MachineArray");
    if (!fs::exists(machineDataPath))
    {
        Logger::log("PackageManager", __func__, INFO, "creating new machineArray.");
        machineDataFileStream.open(machineDataPath, std::ios_base::out);
        machineDataFileStream << L"SERVANT MachineList 0.1" << std::endl;
    } else
    {
        Logger::log("PackageManager", __func__, INFO, "existing machineList found.");
        loadMachinesArray();
    }
}

void PackageManager::initDataDir()
{
    userDataDir = userHomeDir.wstring() + std::wstring(L"SERVANTConfig");
    if (fs::exists(userDataDir))
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

void PackageManager::initHomePath(std::wstring &userHomePath)
{
    auto backslashPos(userHomePath.find(L'\\'));
    while (backslashPos != std::wstring::npos)
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

PackageManager::~PackageManager()
{
    if (machineDataFileStream.is_open())
    {
        machineDataFileStream.close();
    }
    instance = nullptr;
}

void PackageManager::loadMachinesArray()
{
    machines.clear();
    if (machineDataFileStream.is_open())
    {
        machineDataFileStream.close();
    }
    machineDataFileStream.open(machineDataPath, std::ios::in);
    std::wstring versionString;
    std::getline(machineDataFileStream, versionString);
    Logger::log("PackageManager", __func__, INFO, versionString);

    std::wstring dataString;
    while (!machineDataFileStream.eof())
    {
        dataString.clear();
        std::getline(machineDataFileStream, dataString);
        auto columnFlagPos = dataString.find(L",");
        if (columnFlagPos != std::wstring::npos)
        {
            auto newName(dataString.substr(0, columnFlagPos)),
                    newUuid(dataString.substr(columnFlagPos + 1));

            if (VBoxWrapperClient::getInstance()->message()->message(L"select search " + newName).find(L"SUCCEED") ==
                std::string::npos)
            {
                //TODO update config file
                Logger::log("PackageManager", __func__, INFO,
                            L"Invalid machine UUID " + newUuid + L", this machine will not be loaded.");
                continue;
            }

            Logger::log("PackageManager", __func__, INFO,
                        L"Name = " + newName + L", Uuid = " + newUuid);
            VirtualMachine newMachine;
            newMachine.setName(newName);
            newMachine.setUuid(newUuid);

            machines.push_back(newMachine);
        } else
        {
            Logger::log("PackageManager", __func__, INFO, L"Invalid Data, Skipped.");
        }
    }
}

std::wstring PackageManager::importOVA(std::wstring path)
{
    return client->message()->message(L"import " + path);
}

bool PackageManager::OVAImportCompleted()
{
    auto result = client->message()->message(L"get importStat");
    return (result == L"IDLE");
}

bool PackageManager::OVAImportSucceeded()
{
    auto resultWString = client->message()->message(L"get importSucceeded");
    auto succeeded = (resultWString == L"true");
    if (succeeded)
    {
        auto newName(client->message()->message(L"get importedNewName"));
        Logger::log("PackageManager", __func__, INFO, L"New Server is named " + newName);
        if (machineDataFileStream.is_open())
        {
            machineDataFileStream.close();
            machineDataFileStream.open(machineDataPath, std::ios::out | std::ios::app);
        }
        machineDataFileStream << newName << L","
                              << client->message()->machineMessage(newName, L"get machineId")
                              << std::endl;
        machineDataFileStream.close();
        loadMachinesArray();
    }
    return succeeded;
}

int PackageManager::OVAImportProgress()
{
    auto result = client->message()->message(L"get importProgress");
    return std::stoi(result);
}

std::vector<VirtualMachine> *PackageManager::getMachines()
{
    return &machines;
}

std::string PackageManager::getUserDataDir()
{
    return userDataDir.string();
}

std::wstring PackageManager::getUserDataDirWstring()
{
    return userDataDir.wstring();
}

void PackageManager::DeleteMachine(VirtualMachine *virtualMachine)
{
    //TODO fail
    auto machineUuid(virtualMachine->getUuid());
    DeleteMachine(machineUuid);
}

void PackageManager::DeleteMachine(const std::wstring &nameOrUuid)
{
    Logger::log("PackageManager", __func__, INFO,
                L"Deleting " + nameOrUuid + L", vboxWrapper may freeze for one second.");
//    TODO fail
    client->message()->message(L"del " + nameOrUuid);
    for (auto it = machines.begin(); it != machines.end(); ++it)
    {
        if (it->getUuid() == nameOrUuid || it->getName() == nameOrUuid)
        {
            --it;
            machines.erase(it + 1);
        }
    }
}

bool PackageManager::isFirstTime()
{
    return firstTime;
}

PackageManager *PackageManager::getInstance()
{
    return instance;
}

std::string PackageManager::getRemoteServiceHost()
{
    std::fstream RemoteServerFileStream;
    if(!fs::exists(getUserDataDir() + "RemoteServer"))
    {
        RemoteServerFileStream.open(getUserDataDir() + "RemoteServer", std::ios_base::out);
        RemoteServerFileStream << "localhost" << std::endl;
        RemoteServerFileStream.close();
        return "localhost";
    } else
    {
        std::string remoteHost;
        RemoteServerFileStream.open(getUserDataDir() + "RemoteServer", std::ios_base::in);
        RemoteServerFileStream >> remoteHost;
        return remoteHost;
    }
}
