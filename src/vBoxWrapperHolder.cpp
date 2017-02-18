#include "vBoxWrapperClient.hpp"
#include "vBoxWrapperHolder.hpp"
#include "logger.hpp"
#include <experimental/filesystem>
#include <thread>

namespace fs = std::experimental::filesystem;

vBoxWrapperHolder *vBoxWrapperHolder::instance(nullptr);

vBoxWrapperHolder::vBoxWrapperHolder()
{
    fs::path defaultPath(fs::current_path().string() + std::string("/vboxWrapper.exe"));
    if (!fs::exists(defaultPath))
    {
        logger::log("vBoxWrapperHolder", __func__, InfoLevel::WARNING,
                    "Can not start vBoxWrapper server from " + defaultPath.string());
//        TODO find a better way
//        throw std::runtime_error("Can not find vboxWrapper executable, ask GUI to provide it");
        return;
    }
    vBoxWrapperPath = defaultPath.string();
    //TODO extract to new function
    logger::log("vBoxWrapperHolder", __func__, InfoLevel::INFO,
                "vBoxWrapper's default path is" + defaultPath.string());
    instance = this;
}

vBoxWrapperHolder::vBoxWrapperHolder(std::string path)
{
    if (!fs::exists(path))
    {
        logger::log("vBoxWrapperHolder", __func__, InfoLevel::WARNING, "Can not start vBoxWrapper server from " + path);
        return;
//        throw std::runtime_error("Can not find vboxWrapper executable, ask GUI to provide it");
    }
    vBoxWrapperPath = path;
    logger::log("vBoxWrapperHolder", __func__, InfoLevel::INFO, "vBoxWrapper's path is" + path);
    instance = this;
}

vBoxWrapperHolder::~vBoxWrapperHolder()
{
    if (isRunning())
    {
        vBoxWrapperClient::getInstance()->message()->message(L"exit");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for (auto i = 0; i < 5 && wrapperRunning; ++i)
        {
            vBoxWrapperClient::getInstance()->message()->message(L"exit");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (i == 4 && wrapperRunning)
            {
                logger::log("vBoxWrapperHolder", __func__, InfoLevel::ERR, "Can not stop vBoxWrapperClient normally!");
            }
        }
    }
    instance = nullptr;
}

bool vBoxWrapperHolder::isRunning()
{
    return wrapperRunning;
}

void vBoxWrapperHolder::workerThread()
{
    while (vBoxWrapper.isRunning())
    {
        if (vBoxWrapper.getStdOut()->find("\n") != std::string::npos)
        {
            //TODO Solve conflicts
            vBoxWrapper.getStdOut()->resize(vBoxWrapper.getStdOut()->size() - 1);
            logger::log("vBoxWrapperHolder", __func__, InfoLevel::INFO, *vBoxWrapper.getStdOut());
            vBoxWrapper.getStdOut()->clear();
        }
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //TODO implement exit code
    wrapperRunning = false;
}

void vBoxWrapperHolder::start()
{
    if (isRunning() || vBoxWrapperPath.empty())
        return;
    vBoxWrapper.setCmdLine(vBoxWrapperPath.c_str());
    vBoxWrapper.run();
    if (!vBoxWrapper.isRunning())
    {
        throw std::runtime_error("Can not start VBoxWrapper process");
    }
    //TODO security check
    logger::log("vBoxWrapperHolder", __func__, InfoLevel::INFO, "starting vBoxWrapper");
    wrapperRunning = true;
    std::thread(&vBoxWrapperHolder::workerThread, this).detach();
}

vBoxWrapperHolder *vBoxWrapperHolder::getInstance()
{
    if (instance)
        return instance;
    throw std::runtime_error("no instance created.");
}

