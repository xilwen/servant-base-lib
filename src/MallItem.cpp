#include "MallItem.hpp"
#include "HtmlFileDownloader.hpp"
#include "Logger.hpp"
#include "ProfileManager.hpp"
#include "ConfigManager.hpp"
#include <chrono>
#include <thread>

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

const std::string &MallItem::getFileSize() const
{
    return fileSize;
}

void MallItem::setFileSize(const std::string &fileSize)
{
    MallItem::fileSize = fileSize;
}


void MallItem::downloadAdditionalFiles()
{
    HtmlFileDownloader *htmlFileDownloader = HtmlFileDownloader::getInstance();
    while(htmlFileDownloader->isDownloading())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Logger::log("MallItem", __func__, InfoLevel::WARNING, "waiting for other downloads...");
    }

    htmlFileDownloader->startDownload(ConfigManager::getInstance()->getRemoteServiceHost() + "/" + getIconPath(),
                                      ProfileManager::getInstance()->getUserDataDirString() + "/" + getIconPath());
    while(htmlFileDownloader->isDownloading())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

}

bool MallItem::filesAreReady()
{

}

void MallItem::installAdditionalInfoToLastInstalledMachine()
{
    json j(ProfileManager::getInstance()->getMachinesJson());
    if(!j.is_array())
    {
        Logger::log("MallItem", __func__, InfoLevel::ERR, "no machine in machine json. stop.");
        return;
    }
    j.back().emplace("serverType", getName());
    j.back().emplace("icon", getIconPath());
    j.back().emplace("port", getPort());
    j.back().emplace("managementURL", getManagementURL());
    j.back().emplace("managementPort", getManagementPort());
    j.back().emplace("tipURL", getTipURL());
    j.back().emplace("onlyOneServerAllowed", isOnlyOneServerAllowed());
    j.back().emplace("protocol", getProtocol());
    j.back().emplace("singletonPort", getSingletonPort());

    ProfileManager::getInstance()->writeMachinesJson(j);
}

const std::string &MallItem::getManagementURL() const
{
    return managementURL;
}

void MallItem::setManagementURL(const std::string &managementURL)
{
    MallItem::managementURL = managementURL;
}

unsigned int MallItem::getPort() const
{
    return port;
}

void MallItem::setPort(unsigned int port)
{
    MallItem::port = port;
}

const std::string &MallItem::getShareAdditionURL() const
{
    return shareAdditionURL;
}

void MallItem::setShareAdditionURL(const std::string &shareAdditionURL)
{
    MallItem::shareAdditionURL = shareAdditionURL;
}

unsigned int MallItem::getManagementPort() const
{
    return managementPort;
}

void MallItem::setManagementPort(unsigned int managementPort)
{
    MallItem::managementPort = managementPort;
}

unsigned int MallItem::getProfile0CPU() const
{
    return profile0CPU;
}

void MallItem::setProfile0CPU(unsigned int profile0CPU)
{
    MallItem::profile0CPU = profile0CPU;
}

unsigned int MallItem::getProfile0RAM() const
{
    return profile0RAM;
}

void MallItem::setProfile0RAM(unsigned int profile0RAM)
{
    MallItem::profile0RAM = profile0RAM;
}

unsigned int MallItem::getProfile1CPU() const
{
    return profile1CPU;
}

void MallItem::setProfile1CPU(unsigned int profile1CPU)
{
    MallItem::profile1CPU = profile1CPU;
}

unsigned int MallItem::getProfile1RAM() const
{
    return profile1RAM;
}

void MallItem::setProfile1RAM(unsigned int profile1RAM)
{
    MallItem::profile1RAM = profile1RAM;
}

unsigned int MallItem::getProfile2CPU() const
{
    return profile2CPU;
}

void MallItem::setProfile2CPU(unsigned int profile2CPU)
{
    MallItem::profile2CPU = profile2CPU;
}

unsigned int MallItem::getProfile2RAM() const
{
    return profile2RAM;
}

void MallItem::setProfile2RAM(unsigned int profile2RAM)
{
    MallItem::profile2RAM = profile2RAM;
}

const std::string &MallItem::getIntroductionURL() const
{
    return introductionURL;
}

void MallItem::setIntroductionURL(const std::string &introductionURL)
{
    MallItem::introductionURL = introductionURL;
}

const std::string &MallItem::getProtocol() const
{
    return protocol;
}

void MallItem::setProtocol(const std::string &protocol)
{
    MallItem::protocol = protocol;
}

const std::string &MallItem::getTipURL() const
{
    return tipURL;
}

void MallItem::setTipURL(const std::string &tipURL)
{
    MallItem::tipURL = tipURL;
}

bool MallItem::isOnlyOneServerAllowed() const
{
    return onlyOneServerAllowed;
}

void MallItem::setOnlyOneServerAllowed(bool onlyOneServerAllowed)
{
    MallItem::onlyOneServerAllowed = onlyOneServerAllowed;
}

unsigned int MallItem::getSingletonPort() const
{
    return singletonPort;
}

void MallItem::setSingletonPort(unsigned int singletonPort)
{
    MallItem::singletonPort = singletonPort;
}
