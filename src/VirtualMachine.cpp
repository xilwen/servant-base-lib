#include "VirtualMachine.hpp"
#include "VBoxWrapperClient.hpp"
#include "PackageManager.hpp"
#include "ProfileManager.hpp"
#include <json.hpp>
using json = nlohmann::json;

VirtualMachine::VirtualMachine()
{

}

VirtualMachine::~VirtualMachine()
{

}

void VirtualMachine::setUuid(std::wstring uuid)
{
    this->uuid = uuid;
}

void VirtualMachine::setName(std::wstring name)
{
    this->name = name;
}

std::wstring VirtualMachine::getUuid()
{
    return uuid;
}

std::wstring VirtualMachine::getName()
{
    return name;
}

std::wstring VirtualMachine::launch()
{
    return VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"control start");
}

std::wstring VirtualMachine::sendPowerOffSignal()
{
    return VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"control stop");
}

void VirtualMachine::rename(std::wstring newName)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"set machineName " + newName);
}

VirtualMachineState VirtualMachine::getStatus()
{
    auto result = VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"get machineState");
    if (result == L"PoweredOff")
    {
        return VirtualMachineState::PoweredOff;
    }
    if (result == L"Running")
    {
        return VirtualMachineState::Running;
    }
    if (result == L"Aborted")
    {
        return VirtualMachineState::Aborted;
    }
    if (result == L"Starting")
    {
        return VirtualMachineState::Starting;
    }
    if (result == L"Stopping")
    {
        return VirtualMachineState::Stopping;
    } else
    {
        return VirtualMachineState::Unknown;
    }
}

VirtualMachine *VirtualMachine::getVirtualMachine(const std::wstring &nameOrUuid)
{
    std::vector<VirtualMachine> *machines = PackageManager::getInstance()->getMachines();
    for (VirtualMachine x : *machines)
    {
        if (x.getName() == nameOrUuid || x.getUuid() == nameOrUuid)
        {
            return x.getInstance();
        }
    }
    return nullptr;
}

VirtualMachine *VirtualMachine::getInstance()
{
    return this;
}

std::string &VirtualMachine::getIconPath()
{
    return iconPath;
}

void VirtualMachine::setIconPath(const std::string &iconPath)
{
    VirtualMachine::iconPath = iconPath;
}

std::string &VirtualMachine::getType()
{
    return type;
}

void VirtualMachine::setType(const std::string &type)
{
    VirtualMachine::type = type;
}

unsigned int VirtualMachine::getPortNumber() const
{
    return portNumber;
}

void VirtualMachine::setPortNumber(unsigned int portNumber)
{
    VirtualMachine::portNumber = portNumber;
}

unsigned int VirtualMachine::getCustomPortNumber() const
{
    return customPortNumber;
}

void VirtualMachine::setCustomPortNumber(unsigned int customPortNumber)
{
    VirtualMachine::customPortNumber = customPortNumber;
}

const std::string &VirtualMachine::getManagementURL() const
{
    return managementURL;
}

void VirtualMachine::setManagementURL(const std::string &managementURL)
{
    VirtualMachine::managementURL = managementURL;
}

const std::string &VirtualMachine::getShareURL() const
{
    return shareURL;
}

void VirtualMachine::setShareURL(const std::string &shareURL)
{
    VirtualMachine::shareURL = shareURL;
}

const std::string &VirtualMachine::getShareAdditionURL() const
{
    return shareAdditionURL;
}

void VirtualMachine::setShareAdditionURL(const std::string &shareAdditionURL)
{
    VirtualMachine::shareAdditionURL = shareAdditionURL;
}

unsigned int VirtualMachine::getManagementPort() const
{
    return managementPort;
}

void VirtualMachine::setManagementPort(unsigned int managementPort)
{
    VirtualMachine::managementPort = managementPort;
}

const std::string &VirtualMachine::getProtocol() const
{
    return protocol;
}

void VirtualMachine::setProtocol(const std::string &protocol)
{
    VirtualMachine::protocol = protocol;
}

const std::string &VirtualMachine::getTipURL() const
{
    return tipURL;
}

void VirtualMachine::setTipURL(const std::string &tipURL)
{
    VirtualMachine::tipURL = tipURL;
}

bool VirtualMachine::isUseLocalIP() const
{
    return useLocalIP;
}

void VirtualMachine::setUseLocalIP(bool useLocalIP)
{
    VirtualMachine::useLocalIP = useLocalIP;
}

void VirtualMachine::setCPUAmount(int cpuAmount)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"set cpuCount " + std::to_wstring(cpuAmount));
}

void VirtualMachine::setRAMAmount(int ramAmount)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"set ramSize " + std::to_wstring(ramAmount));

}

void VirtualMachine::addPortForwardingRule(unsigned int guestPort, unsigned int hostPort)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid,
                                                                L"set addPortForwarding " + std::to_wstring(guestPort) +
                                                                L" " + std::to_wstring(hostPort));
}

void VirtualMachine::removePortForwardingRule(unsigned int guestPort)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"set removePortForwarding " +
                                                                      std::to_wstring(guestPort));
}

void VirtualMachine::saveUseLocalIP(bool useLocalIP)
{
    json machineJson(ProfileManager::getInstance()->getMachinesJson());
    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        if(it->find("machineUuid").value() == this->getUuidString())
        {
            if(it->find("useLocalIP") != it->end())
            {
                it->at("useLocalIP") = useLocalIP;
            } else
            {
                it->emplace("useLocalIP", useLocalIP);
            }
        }
    }
    ProfileManager::getInstance()->writeMachinesJson(machineJson);
}

const std::string &VirtualMachine::getUuidString() const
{
    return uuidString;
}

void VirtualMachine::setUuidString(const std::string &uuidString)
{
    VirtualMachine::uuidString = uuidString;
}

void VirtualMachine::saveCustomPort(unsigned int customPort)
{
    json machineJson(ProfileManager::getInstance()->getMachinesJson());
    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        if(it->find("machineUuid").value() == this->getUuidString())
        {
            if(it->find("customPort") != it->end())
            {
                it->at("customPort") = customPort;
            } else
            {
                it->emplace("customPort", customPort);
            }
        }
    }
    ProfileManager::getInstance()->writeMachinesJson(machineJson);
}

void VirtualMachine::enableWebmin()
{
    addPortForwardingRule(10000, 10000);

    json machineJson(ProfileManager::getInstance()->getMachinesJson());
    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        if(it->find("machineUuid").value() == this->getUuidString())
        {
            if(it->find("webminLock") != it->end())
            {
                it->at("webminLock") = true;
            } else
            {
                it->emplace("webminLock", true);
            }
        }
    }
    ProfileManager::getInstance()->writeMachinesJson(machineJson);
}

void VirtualMachine::disableWebmin()
{
    removePortForwardingRule(10000);

    json machineJson(ProfileManager::getInstance()->getMachinesJson());
    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        if(it->find("machineUuid").value() == this->getUuidString())
        {
            if(it->find("webminLock") != it->end())
            {
                it->at("webminLock") = false;
            } else
            {
                it->emplace("webminLock", false);
            }
        }
    }
    ProfileManager::getInstance()->writeMachinesJson(machineJson);
}

bool VirtualMachine::webminEnabled()
{
    json machineJson(ProfileManager::getInstance()->getMachinesJson());
    for (auto it = machineJson.begin(); it != machineJson.end(); ++it)
    {
        if(it->find("machineUuid").value() == this->getUuidString())
        {
            if(it->find("webminLock") != it->end())
            {
                if(it->find("webminLock").value() == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void VirtualMachine::exportMachine(std::wstring path)
{
    VBoxWrapperClient::getInstance()->message()->machineMessage(uuid,
                                                                L"control output " + path);
}

unsigned int VirtualMachine::getSingletonPort() const
{
    return singletonPort;
}

void VirtualMachine::setSingletonPort(unsigned int singletonPort)
{
    VirtualMachine::singletonPort = singletonPort;
}
