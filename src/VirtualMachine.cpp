#include "VirtualMachine.hpp"
#include "VBoxWrapperClient.hpp"
#include "PackageManager.hpp"

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

std::string & VirtualMachine::getIconPath()
{
    return iconPath;
}

void VirtualMachine::setIconPath(const std::string &iconPath)
{
    VirtualMachine::iconPath = iconPath;
}

std::string & VirtualMachine::getType()
{
    return type;
}

void VirtualMachine::setType(const std::string &type)
{
    VirtualMachine::type = type;
}

