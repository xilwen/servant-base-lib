#include <thread>
#include "MallManager.hpp"
#include "ProfileManager.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp"
#include "HtmlFileDownloader.hpp"

namespace fs = std::experimental::filesystem;

MallManager::MallManager()
{
    if (!ProfileManager::getInstance()->getMallRepositoryJson().is_null())
    {
        loadLocalItemList();
    }
}

MallManager::~MallManager()
{

}

std::vector<MallItem> *MallManager::getItemList()
{
    return &items;
}

bool MallManager::remoteServerIsReady()
{
    return false;
}

void MallManager::updateItemList()
{
    getListFromRemoteServer();
    loadLocalItemList();
}

void MallManager::getListFromRemoteServer()
{
    auto remotePath(ConfigManager::getInstance()->getRemoteServiceHost() + "/mallRepository.json");
    HtmlFileDownloader::getInstance()->startDownload(remotePath, ProfileManager::getInstance()->getUserDataDir().string() + "/mallRepository.json");
    unsigned int downloadMillisecondsElapsed = 0;
    while (HtmlFileDownloader::getInstance()->isDownloading())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        downloadMillisecondsElapsed += 5;
        if(downloadMillisecondsElapsed > 5000)
        {
            Logger::log("MallManager", __func__, InfoLevel::ERR, "Download time exceed 5 seconds!Cancelling...");
            HtmlFileDownloader::getInstance()->cancelCurrentDownload();
        }
    }
    if(!HtmlFileDownloader::getInstance()->succeededLastTime())
    {
        Logger::log("MallManager", __func__, InfoLevel::ERR, "Download failed!");
        ProfileManager::getInstance()->writeMallRepositoryJson("[]"_json);
    } else
    {
        Logger::log("MallManager", __func__, InfoLevel::ERR, "Download succeed!");
    }
}

void MallManager::loadLocalItemList()
{
    items.clear();
    json j(ProfileManager::getInstance()->getMallRepositoryJson());
    for (auto it = j.begin(); it != j.end(); ++it)
    {
        MallItem newItem;
        try
        {
            newItem.setName(it->find("name").value());
            newItem.setDescription(it->find("description").value());
            newItem.setProducer(it->find("producer").value());
            newItem.setIconPath(it->find("icon").value());
            newItem.setUpdateDate(it->find("updateDate").value());
            newItem.setOperatingSystem(it->find("operatingSystem").value());
            newItem.setManagementUI(it->find("managementUI").value());
            newItem.setIntroduction(it->find("introduction").value());
            newItem.setCpuLevel(it->find("cpuLevel").value());
            newItem.setRamLevel(it->find("ramLevel").value());
            newItem.setDiskSpeedLevel(it->find("diskSpeedLevel").value());
            newItem.setRequireVMX(it->find("requireVMX").value());
            newItem.setVboxImageFile(it->find("vboxImageFile").value());
            newItem.setFileSize(it->find("fileSize").value());
            newItem.setPort(it->find("port").value());
            newItem.setManagementPort(it->find("managementPort").value());
            newItem.setManagementURL(it->find("managementURL").value());
            newItem.setProtocol(it->find("protocol").value());
            newItem.setTipURL(it->find("tipURL").value());
            if(it->find("shareAdditionURL") != it->end())
            {
                newItem.setShareAdditionURL(it->find("shareAdditionURL").value());
            }
            if(it->find("profile0CPU") != it->end())
            {
                newItem.setProfile0CPU(it->find("profile0CPU").value());
                newItem.setProfile0RAM(it->find("profile0RAM").value());
            }
            if(it->find("profile1CPU") != it->end())
            {
                newItem.setProfile1CPU(it->find("profile1CPU").value());
                newItem.setProfile1RAM(it->find("profile1RAM").value());
            }
            if(it->find("profile2CPU") != it->end())
            {
                newItem.setProfile2CPU(it->find("profile2CPU").value());
                newItem.setProfile2RAM(it->find("profile2RAM").value());
            }
            if(it->find("introductionURL") != it->end())
            {
                newItem.setIntroductionURL(it->find("introductionURL").value());
            }
            if(it->find("onlyOneServerAllowed") != it->end())
            {
                newItem.setOnlyOneServerAllowed(it->find("onlyOneServerAllowed").value());
            }
            if(it->find("singletonPort") != it->end())
            {
                newItem.setSingletonPort(it->find("singletonPort").value());
            }
        }
        catch (std::exception &e)
        {
            Logger::log("MallManager", __func__, InfoLevel::WARNING, "Invalid Entry, Skipped");
            continue;
        }
        items.push_back(newItem);
    }
}

