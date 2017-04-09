#ifndef TESTEXE_HTMLFILEDOWNLOADER_H
#define TESTEXE_HTMLFILEDOWNLOADER_H

#include <string>

class HtmlFileDownloader
{
public:
    static HtmlFileDownloader *getInstance();

    void startDownload(std::string url, std::string localPath);

    bool isDownloading();

    unsigned int downloadProgress();

    bool succeededLastTime();

    void cancelCurrentDownload();

    virtual ~HtmlFileDownloader();

private:
    HtmlFileDownloader();

    void *curl = nullptr;
    const std::string className = "HtmlFileDownloader";
    bool downloading = false,
            succeeded = false;
    unsigned int progress = 0;
    FILE *fp;
    static bool exist;
    static bool cancelled;
    static HtmlFileDownloader *instance;

    void runDownloadThread();

    static int
    progressHandler(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpload, double nowUploaded);

    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

    void setProgress(unsigned int progress);

};

#endif //TESTEXE_HTMLFILEDOWNLOADER_H
