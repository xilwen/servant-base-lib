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
    } else
    {
        Logger::log("MallManager", __func__, InfoLevel::ERR, "Download succeed!");
    }
}

void MallManager::loadLocalItemList()
{
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
        }
        catch (std::exception &e)
        {
            Logger::log("MallManager", __func__, InfoLevel::WARNING, "Invalid Entry, Skipped");
            continue;
        }
        items.push_back(newItem);
    }
}

