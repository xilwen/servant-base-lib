#include "PackageManager.hpp"
#include "Logger.hpp"
#include "ProfileManager.hpp"
#include <json.hpp>
#include <boost/locale.hpp>

using json = nlohmann::json;

namespace fs = std::experimental::filesystem;

PackageManager *PackageManager::instance(nullptr);

PackageManager::PackageManager()
{
    instance = this;
    userDataDir = ProfileManager::getInstance()->getUserDataDir();
    vBoxWrapperClient = VBoxWrapperClient::getInstance();
    initMachineData();
}

void PackageManager::initMachineData()
{
    std::__cxx11::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    machineDataPath = converter.to_bytes(userDataDir.wstring() + L"/machines.json");
    if (!fs::exists(machineDataPath))
    {
        Logger::log("PackageManager", __func__, INFO, "creating new machines.json.");
        machineDataFileStream.open(machineDataPath, std::ios::out | std::ios::trunc);
        "[]"_json >> machineDataFileStream;
        machineDataFileStream.close();
    } else
    {
        Logger::log("PackageManager", __func__, INFO, "existing machines.json found.");
        loadMachinesArray();
    }
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
    json machineJson;
    machineDataFileStream >> machineJson;
    machineDataFileStream.close();

    if (!machineJson.is_array())
    {
        Logger::log("PackageManager", __func__, InfoLevel::ERR, L"Invalid machines.json. Nothing will be load!");
        //TODO do something if fail
        return;
    }

    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        try
        {
            std::string machineUuidString = it->find("machineUuid").value(); //utf-8 string from json
            auto machineUuidWstring(boost::locale::conv::utf_to_utf<wchar_t>(machineUuidString));
            if (vBoxWrapperClient->message()->message(L"select search " + machineUuidWstring).find(L"SUCCEED") ==
                std::string::npos)
            {
                //TODO update config file
                Logger::log("PackageManager", __func__, INFO,
                            L"Invalid machine UUID " + machineUuidWstring + L", this machine will not be loaded.");
                continue;
            }
            auto machineNameWstring(vBoxWrapperClient->message()->message(L"get machineName"));
            Logger::log("PackageManager", __func__, INFO,
                        L"Name = " + machineNameWstring + L", Uuid = " + machineUuidWstring);
            VirtualMachine newMachine;
            newMachine.setName(machineNameWstring);
            newMachine.setUuid(machineUuidWstring);
            machines.push_back(newMachine);
        } catch (std::exception &e)
        {
            Logger::log("PackageManager", __func__, INFO, e.what());
        }
    }
}

std::wstring PackageManager::importOVA(std::wstring path)
{
    return vBoxWrapperClient->message()->message(L"import " + path);
}

bool PackageManager::OVAImportCompleted()
{
    auto result = vBoxWrapperClient->message()->message(L"get importStat");
    return (result == L"IDLE");
}

bool PackageManager::OVAImportSucceeded()
{
    auto resultWString = vBoxWrapperClient->message()->message(L"get importSucceeded");
    auto succeeded = (resultWString == L"true");
    if (succeeded)
    {
        auto newName(vBoxWrapperClient->message()->message(L"get importedNewName"));
        Logger::log("PackageManager", __func__, INFO, L"New Server is named " + newName);
        if (machineDataFileStream.is_open())
        {
            machineDataFileStream.close();
        }
        machineDataFileStream.open(machineDataPath, std::ios::in);
        json machineJson;
        machineDataFileStream >> machineJson;
        machineDataFileStream.close();

        if (machineJson.is_array())
        {
            machineJson.push_back(json::object());
            std::string newUuidString = boost::locale::conv::utf_to_utf<char>(
                    vBoxWrapperClient->message()->machineMessage(newName, L"get machineId"));
            machineJson.back().emplace("machineUuid", newUuidString.c_str());
            machineDataFileStream.open(machineDataPath, std::ios::out | std::ios::trunc);
            machineDataFileStream << machineJson;
            machineDataFileStream.close();
        }
        loadMachinesArray();
    }
    return succeeded;
}

int PackageManager::OVAImportProgress()
{
    auto result = vBoxWrapperClient->message()->message(L"get importProgress");
    return std::stoi(result);
}

std::vector<VirtualMachine> *PackageManager::getMachines()
{
    return &machines;
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
    vBoxWrapperClient->message()->message(L"del " + nameOrUuid);
    for (auto it = machines.begin(); it != machines.end(); ++it)
    {
        if (it->getUuid() == nameOrUuid || it->getName() == nameOrUuid)
        {
            --it;
            machines.erase(it + 1);
        }
    }
}

PackageManager *PackageManager::getInstance()
{
    return instance;
}

