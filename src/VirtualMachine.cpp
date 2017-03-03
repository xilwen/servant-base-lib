#include "VirtualMachine.hpp"
#include "VBoxWrapperClient.hpp"

VirtualMachine::VirtualMachine()
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
