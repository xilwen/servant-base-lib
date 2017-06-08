#include "PackageManager.hpp"
#include "Logger.hpp"
#include "ProfileManager.hpp"
#include "ConfigManager.hpp"
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
    json j(ProfileManager::getInstance()->getMachinesJson());
    if(!j.is_array())
    {
        Logger::log("PackageManager", __func__, INFO, "JSON Invalid, will be recreated.");
        ProfileManager::getInstance()->resetMachinesJson();
        return;
    }
    if(j.size() == 0)
    {
        return;
    }
    loadMachinesArray();
}

PackageManager::~PackageManager()
{
    instance = nullptr;
}

void PackageManager::loadMachinesArray()
{
    machines.clear();

    json machineJson(ProfileManager::getInstance()->getMachinesJson());

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
            newMachine.setUuidString(it->find("machineUuid").value());
            if(it->find("icon") != it->end())
            {
                newMachine.setIconPath(ConfigManager::getInstance()->getRemoteServiceHost() + "/" + it->find("icon")->get<std::string>());
            }
            if(it->find("serverType") != it->end())
            {
                newMachine.setType(it->find("serverType").value());
            }
            if(it->find("port") != it->end())
            {
                newMachine.setPortNumber(it->find("port").value());
            }
            if(it->find("customPort") != it->end())
            {
                newMachine.setCustomPortNumber(it->find("customPort").value());
            }
            if(it->find("managementURL") != it->end())
            {
                newMachine.setManagementURL(it->find("managementURL").value());
                newMachine.setManagementPort(it->find("managementPort").value());
            }
            if(it->find("tipURL") != it->end())
            {
                newMachine.setTipURL(it->find("tipURL").value());
            }
            if(it->find("protocol") != it->end())
            {
                newMachine.setProtocol(it->find("protocol").value());
            }
            if(it->find("managementPort") != it->end())
            {
                newMachine.setManagementPort(it->find("managementPort").value());
            }
            if(it->find("useLocalIP") != it->end())
            {
                newMachine.setUseLocalIP(it->find("useLocalIP").value());
            }
            if(it->find("singletonPort") != it->end())
            {
                newMachine.setSingletonPort(it->find("singletonPort").value());
            }
            machines.push_back(newMachine);
        } catch (std::exception &e)
        {
            Logger::log("PackageManager", __func__, INFO, e.what());
        }
    }

    //Log Machines Array for debugging
    std::wstring logString(L"Machines Array Info:\n");
    for (auto it = machines.begin(); it != machines.end(); ++it)
    {
        logString += ( it->getName() + L" " + it->getUuid() + L"\n");
    }
    logString += L"****";
    Logger::log("PackageManager", __func__, InfoLevel::INFO, logString);
}

std::wstring PackageManager::importOVA(std::wstring path)
{
    Logger::log("PackageManager", __func__, InfoLevel::INFO, path);
    return vBoxWrapperClient->message()->message(L"import " + path);
}

bool PackageManager::OVAImportCompleted()
{
    Logger::log("PackageManager", __func__, InfoLevel::INFO, "Import Completed");
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

        json machineJson(ProfileManager::getInstance()->getMachinesJson());

        if (machineJson.is_array())
        {
            machineJson.push_back(json::object());
            std::string newUuidString = boost::locale::conv::utf_to_utf<char>(
                    vBoxWrapperClient->message()->machineMessage(newName, L"get machineId"));
            machineJson.back().emplace("machineUuid", newUuidString.c_str());
            ProfileManager::getInstance()->writeMachinesJson(machineJson);
        }
        loadMachinesArray();
    } else
    {
        Logger::log("PackageManager", __func__, InfoLevel::INFO, "Import Failed");
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

void PackageManager::refreshMachines()
{
    loadMachinesArray();
}
