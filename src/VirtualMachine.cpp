#include "VirtualMachine.h"
#include "vBoxWrapperClient.h"

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
    return vBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"control start");
}

std::wstring VirtualMachine::sendPowerOffSignal()
{
    return vBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"control stop");
}

void VirtualMachine::rename(std::wstring newName)
{
    vBoxWrapperClient::getInstance()->message()->machineMessage(uuid, L"set machineName " + newName);

}
