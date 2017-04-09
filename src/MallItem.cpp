#include "MallItem.hpp"

const std::string &MallItem::getName() const
{
    return name;
}

const std::string &MallItem::getDescription() const
{
    return description;
}

const std::string &MallItem::getVboxImageFile() const
{
    return vboxImageFile;
}

void MallItem::setVboxImageFile(const std::string &vboxImageFile)
{
    MallItem::vboxImageFile = vboxImageFile;
}

const std::string &MallItem::getProducer() const
{
    return producer;
}

const std::string &MallItem::getIconPath() const
{
    return iconPath;
}

const std::string &MallItem::getUpdateDate() const
{
    return updateDate;
}

const std::string &MallItem::getOperatingSystem() const
{
    return operatingSystem;
}

const std::string &MallItem::getManagementUI() const
{
    return managementUI;
}

const std::string &MallItem::getIntroduction() const
{
    return introduction;
}

void MallItem::setName(const std::string &name)
{
    MallItem::name = name;
}

void MallItem::setDescription(const std::string &description)
{
    MallItem::description = description;
}

void MallItem::setProducer(const std::string &producer)
{
    MallItem::producer = producer;
}

void MallItem::setIconPath(const std::string &iconPath)
{
    MallItem::iconPath = iconPath;
}

void MallItem::setUpdateDate(const std::string &updateDate)
{
    MallItem::updateDate = updateDate;
}

void MallItem::setOperatingSystem(const std::string &operatingSystem)
{
    MallItem::operatingSystem = operatingSystem;
}

void MallItem::setManagementUI(const std::string &managementUI)
{
    MallItem::managementUI = managementUI;
}

void MallItem::setIntroduction(const std::string &introduction)
{
    MallItem::introduction = introduction;
}

int MallItem::getCpuLevel() const
{
    return cpuLevel;
}

void MallItem::setCpuLevel(int cpuLevel)
{
    MallItem::cpuLevel = cpuLevel;
}

int MallItem::getRamLevel() const
{
    return ramLevel;
}

void MallItem::setRamLevel(int ramLevel)
{
    MallItem::ramLevel = ramLevel;
}

int MallItem::getDiskSpeedLevel() const
{
    return diskSpeedLevel;
}

void MallItem::setDiskSpeedLevel(int diskSpeedLevel)
{
    MallItem::diskSpeedLevel = diskSpeedLevel;
}

bool MallItem::isRequireVMX() const
{
    return requireVMX;
}

void MallItem::setRequireVMX(bool requireVMX)
{
    MallItem::requireVMX = requireVMX;
}

