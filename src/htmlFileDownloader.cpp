#include <stdexcept>
#include "htmlFileDownloader.h"
#include "logger.h"
#include <curl.h>
#include <thread>

bool htmlFileDownloader::exist(false);
htmlFileDownloader *htmlFileDownloader::instance(nullptr);

htmlFileDownloader::htmlFileDownloader()
{
    if (exist)
    {
        throw std::runtime_error("only one instance is allowed");
    }
    curl_global_init(CURL_GLOBAL_WIN32);
    curl = curl_easy_init();
    if (!curl)
    {
        logger::log(className, __func__, InfoLevel::ERR, "curl cannot be inited");
        throw std::runtime_error("curl cannot be inited");
    } else
    {
        instance = this;
        exist = true;
    }
}

htmlFileDownloader::~htmlFileDownloader()
{
    curl_easy_cleanup(curl);
    instance = nullptr;
    exist = false;
}

void htmlFileDownloader::startDownload(std::string url, std::string localPath)
{
    if (downloading)
    {
        throw std::runtime_error("already downloading another file");
    }
    logger::log(className, __func__, InfoLevel::INFO, "downloading from " + url);
    logger::log(className, __func__, InfoLevel::INFO, "save to " + localPath);
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
    std::thread downloadThread(&htmlFileDownloader::runDownloadThread, this);
    downloadThread.detach();
}

bool htmlFileDownloader::isDownloading()
{
    return downloading;
}

unsigned int htmlFileDownloader::downloadProgress()
{
    return progress;
}

bool htmlFileDownloader::succeededLastTime()
{
    return succeeded;
}

void htmlFileDownloader::runDownloadThread()
{
    downloading = true;
    CURLcode result;
    result = curl_easy_perform(curl);

    succeeded = (result == CURLE_OK);
    if (fp)
        fclose(fp);
    downloading = false;
}

size_t htmlFileDownloader::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int htmlFileDownloader::progressHandler(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpload,
                                        double nowUploaded)
{
    htmlFileDownloader::returnExist()->setProgress((unsigned int)(nowDownloaded / totalToDownload * 100));
    //TODO implement cancel
    //Always return 0 until cancel function implemented, refers to the cURL document.
    return 0;
}

htmlFileDownloader *htmlFileDownloader::returnExist()
{
    return instance;
}

void htmlFileDownloader::setProgress(unsigned int progress)
{
    htmlFileDownloader::progress = progress;
}
