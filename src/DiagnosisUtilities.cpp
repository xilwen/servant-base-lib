#include <thread>
#include "DiagnosisUtilities.hpp"
#include "HtmlFileDownloader.hpp"
#include "Logger.hpp"
#include "ProfileManager.hpp"
#include "json.hpp"

using json = nlohmann::json;

DiagnosisUtilities *DiagnosisUtilities::getInstance()
{
    static DiagnosisUtilities diagnosisUtilities;
    return &diagnosisUtilities;
}

DiagnosisUtilities::~DiagnosisUtilities()
{

}

DiagnosisUtilities::DiagnosisUtilities()
{

}

std::string DiagnosisUtilities::getIpFromIpify()
{
    timer = 0;
    while (HtmlFileDownloader::getInstance()->isDownloading())
    {
        if (timer > 5000)
        {
            Logger::log("DiagnosisUtilities", __func__, InfoLevel::WARNING, "wait for last download exceed 5000ms");
            return "N/A(Connection timeout)";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        timer += 50;
    }
    std::string fileName(ProfileManager::getInstance()->getUserDataDirString() + "/ipify.txt");
    HtmlFileDownloader::getInstance()->startDownload("http://api.ipify.org", fileName);
    while (HtmlFileDownloader::getInstance()->isDownloading())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    if (!HtmlFileDownloader::getInstance()->succeededLastTime())
    {
        return "N/A(Connection failed)";
    }

    std::ifstream ifstream(fileName, std::ios::in);
    if (!ifstream.is_open())
    {
        return "N/A(Internal Error)";
    }

    std::string ip;
    ifstream >> ip;
    ifstream.close();
    if (!ip.empty())
    {
        return ip;
    }
    return "N/A(data error)";
}

