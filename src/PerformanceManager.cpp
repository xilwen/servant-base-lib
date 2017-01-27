#include "PerformanceManager.h"
#include "packageManager.h"
#include "logger.h"

namespace fs = std::experimental::filesystem;

PerformanceManager *PerformanceManager::instance(nullptr);

int PerformanceManager::getProcessorRate()
{
    return processorRate;
}

int PerformanceManager::getMemoryRate()
{
    return memoryRate;
}

int PerformanceManager::getDiskRate()
{
    return diskRate;
}

void PerformanceManager::setProcessorRate(int processorRate)
{
    PerformanceManager::processorRate = processorRate;
}

void PerformanceManager::setMemoryRate(int memoryRate)
{
    PerformanceManager::memoryRate = memoryRate;
}

void PerformanceManager::setDiskRate(int diskRate)
{
    PerformanceManager::diskRate = diskRate;
}

PerformanceManager::PerformanceManager()
{
    instance = this;
    std::string perfDataPath(packageManager::getInstance()->getUserDataDir() + "/perfData");
    if (fs::exists(perfDataPath))
    {
        std::ifstream perfDataIfstream;
        perfDataIfstream.open(perfDataPath, std::ios::in);
        for (int i = 0; i < 3 && !perfDataIfstream.eof(); ++i)
        {
            std::string dataString;
            std::getline(perfDataIfstream, dataString);
            switch (i)
            {
                case 0:
                    processorRate = std::stoi(dataString);
                    break;
                case 1:
                    memoryRate = std::stoi(dataString);
                    break;
                case 2:
                    diskRate = std::stoi(dataString);
                    break;
                default:
                    break;
            }
        }
        perfDataIfstream.close();
    } else
    {
        processorRate = memoryRate = diskRate = -1;
    }

}

PerformanceManager::~PerformanceManager()
{
    std::string perfDataPath(packageManager::getInstance()->getUserDataDir() + "/perfData");
    std::ofstream perfDataOfstream;
    perfDataOfstream.open(perfDataPath, std::ios::trunc | std::ios::out);
    if (perfDataOfstream.is_open())
    {
        perfDataOfstream << std::to_string(processorRate) << std::endl << std::to_string(memoryRate) << std::endl <<
                         std::to_string(diskRate);
        perfDataOfstream.close();

    } else
    {
        logger::log("PerformanceManager", __func__, InfoLevel::WARNING,
                    "Can not write performance data, changes will not be saved.");
    }
    instance = nullptr;
}

PerformanceManager *PerformanceManager::getInstance()
{
    return instance;
}

bool PerformanceManager::getVirtualizationSupportStatus()
{
    return (vBoxWrapperClient::getInstance()->message()->message(L"get vmxSupport") == L"true");
}
