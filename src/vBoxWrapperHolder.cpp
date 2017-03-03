#include "VBoxWrapperClient.hpp"
#include "VBoxWrapperHolder.hpp"
#include "Logger.hpp"
#include <experimental/filesystem>
#include <thread>

namespace fs = std::experimental::filesystem;

VBoxWrapperHolder *VBoxWrapperHolder::instance(nullptr);

VBoxWrapperHolder::VBoxWrapperHolder()
{
    fs::path defaultPath(fs::current_path().string() + std::string("/vboxWrapper.exe"));
    if (!fs::exists(defaultPath))
    {
        Logger::log("VBoxWrapperHolder", __func__, InfoLevel::WARNING,
                    "Can not start vBoxWrapper server from " + defaultPath.string());
//        TODO find a better way
//        throw std::runtime_error("Can not find vboxWrapper executable, ask GUI to provide it");
        return;
    }
    vBoxWrapperPath = defaultPath.string();
    //TODO extract to new function
    Logger::log("VBoxWrapperHolder", __func__, InfoLevel::INFO,
                "vBoxWrapper's default path is" + defaultPath.string());
    instance = this;
}

VBoxWrapperHolder::VBoxWrapperHolder(std::string path)
{
    if (!fs::exists(path))
    {
        Logger::log("VBoxWrapperHolder", __func__, InfoLevel::WARNING, "Can not start vBoxWrapper server from " + path);
        return;
//        throw std::runtime_error("Can not find vboxWrapper executable, ask GUI to provide it");
    }
    vBoxWrapperPath = path;
    Logger::log("VBoxWrapperHolder", __func__, InfoLevel::INFO, "vBoxWrapper's path is" + path);
    instance = this;
}

VBoxWrapperHolder::~VBoxWrapperHolder()
{
    Logger::log(className, __func__, InfoLevel::INFO, className + " destruction started.");
    if (isRunning())
    {
        VBoxWrapperClient::getInstance()->message()->message(L"exit");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for (auto i = 0; i < 5 && isRunning(); ++i)
        {
            VBoxWrapperClient::getInstance()->message()->message(L"exit");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (i == 4 && isRunning())
            {
                Logger::log("VBoxWrapperHolder", __func__, InfoLevel::ERR, "Can not stop VBoxWrapperClient normally!");
            }
        }
    }
    wrapperRunning = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    delete vBoxWrapper;
    instance = nullptr;
}

bool VBoxWrapperHolder::isRunning()
{
    if (!vBoxWrapper)
        return false;
    return vBoxWrapper->isRunning();
}

void VBoxWrapperHolder::workerThread()
{
    while (isRunning())
    {
        if (vBoxWrapper->getStdOut()->find("\n") != std::string::npos)
        {
            //TODO Solve conflicts
            vBoxWrapper->getStdOut()->resize(vBoxWrapper->getStdOut()->size() - 1);
            Logger::log("VBoxWrapperHolder", __func__, InfoLevel::INFO, *vBoxWrapper->getStdOut());
            vBoxWrapper->getStdOut()->clear();
        }
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //TODO implement exit code
}

void VBoxWrapperHolder::start()
{
    if (isRunning() || vBoxWrapperPath.empty())
        return;
    vBoxWrapper = new ProgramHolder();
    vBoxWrapper->setCmdLine(vBoxWrapperPath.c_str());
    vBoxWrapper->run();
    if (!isRunning())
    {
        throw std::runtime_error("Can not start VBoxWrapper process");
    }
    //TODO security check
    Logger::log("VBoxWrapperHolder", __func__, InfoLevel::INFO, "starting vBoxWrapper");
    wrapperRunning = true;
    std::thread(&VBoxWrapperHolder::workerThread, this).detach();
}

VBoxWrapperHolder *VBoxWrapperHolder::getInstance()
{
    if (instance)
        return instance;
    throw std::runtime_error("no instance created.");
}

