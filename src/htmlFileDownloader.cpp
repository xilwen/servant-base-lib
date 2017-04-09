#include <stdexcept>
#include "HtmlFileDownloader.hpp"
#include "Logger.hpp"
#include <curl.h>
#include <thread>

bool HtmlFileDownloader::exist(false);
HtmlFileDownloader *HtmlFileDownloader::instance(nullptr);
bool HtmlFileDownloader::cancelled(false);

HtmlFileDownloader *HtmlFileDownloader::getInstance()
{
    static HtmlFileDownloader instance;
    return &instance;
}

HtmlFileDownloader::HtmlFileDownloader()
{
    if (exist)
    {
        throw std::runtime_error("only one instance is allowed");
    }
    curl_global_init(CURL_GLOBAL_WIN32);
    curl = curl_easy_init();
    if (!curl)
    {
        Logger::log(className, __func__, InfoLevel::ERR, "curl cannot be inited");
        throw std::runtime_error("curl cannot be inited");
    } else
    {
        instance = this;
        exist = true;
    }
}

HtmlFileDownloader::~HtmlFileDownloader()
{
    curl_easy_cleanup(curl);
    instance = nullptr;
    exist = false;
}

void HtmlFileDownloader::startDownload(std::string url, std::string localPath)
{
    if (downloading)
    {
        throw std::runtime_error("already downloading another file");
    }
    Logger::log(className, __func__, InfoLevel::INFO, "downloading from " + url);
    Logger::log(className, __func__, InfoLevel::INFO, "save to " + localPath);
    //TODO check if file exists
    fp = fopen(localPath.c_str(), "wb");
    if (!fp)
        throw std::runtime_error("can not initiate new file");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progressHandler);
    downloading = true;
    std::thread downloadThread(&HtmlFileDownloader::runDownloadThread, this);
    downloadThread.detach();
}

bool HtmlFileDownloader::isDownloading()
{
    return downloading;
}

unsigned int HtmlFileDownloader::downloadProgress()
{
    return progress;
}

bool HtmlFileDownloader::succeededLastTime()
{
    return succeeded;
}

void HtmlFileDownloader::runDownloadThread()
{
    downloading = true;
    cancelled = false;
    CURLcode result;
    result = curl_easy_perform(curl);

    succeeded = (result == CURLE_OK);
    if (fp)
        fclose(fp);
    downloading = false;
}

size_t HtmlFileDownloader::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int HtmlFileDownloader::progressHandler(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpload,
                                        double nowUploaded)
{
    HtmlFileDownloader::getInstance()->setProgress((unsigned int)(nowDownloaded / totalToDownload * 100));
    //TODO implement using "CURLOPT_XFERINFODATA" to determine which download to be cancel
    if(cancelled)
    {
        return 1;
    }
    return 0;
}

void HtmlFileDownloader::setProgress(unsigned int progress)
{
    HtmlFileDownloader::progress = progress;
}

void HtmlFileDownloader::cancelCurrentDownload()
{
    cancelled = true;
}
