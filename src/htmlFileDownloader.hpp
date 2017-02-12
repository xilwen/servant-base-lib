#ifndef TESTEXE_HTMLFILEDOWNLOADER_H
#define TESTEXE_HTMLFILEDOWNLOADER_H

#include <string>

class htmlFileDownloader
{
public:
    htmlFileDownloader();

    void startDownload(std::string url, std::string localPath);

    bool isDownloading();

    unsigned int downloadProgress();

    bool succeededLastTime();

    virtual ~htmlFileDownloader();

    static htmlFileDownloader *returnExist();
    void setProgress(unsigned int progress);

private:
    void *curl = nullptr;
    const std::string className = "htmlFileDownloader";
    bool downloading = false,
            succeeded = false;
    unsigned int progress = 0;

    FILE *fp;
    static bool exist;
    static htmlFileDownloader *instance;


    void runDownloadThread();

    static int
    progressHandler(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpload, double nowUploaded);

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

};

#endif //TESTEXE_HTMLFILEDOWNLOADER_H
