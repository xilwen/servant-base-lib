#include "PerformanceManager.hpp"
#include "PackageManager.hpp"
#include "Logger.hpp"

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
    std::string perfDataPath(PackageManager::getInstance()->getUserDataDir() + "/perfData");
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
    writeDataToFile();
    instance = nullptr;
}

void PerformanceManager::writeDataToFile()
{
    std::__cxx11::string perfDataPath(PackageManager::getInstance()->getUserDataDir() + "/perfData");
    std::ofstream perfDataOfstream;
    perfDataOfstream.open(perfDataPath, std::ios_base::trunc | std::ios_base::out);
    if (perfDataOfstream.is_open())
    {
        perfDataOfstream << std::__cxx11::to_string(processorRate) << std::endl << std::__cxx11::to_string(memoryRate) << std::endl <<
                         std::__cxx11::to_string(diskRate);
        perfDataOfstream.close();

    } else
    {
        Logger::log("PerformanceManager", __func__, WARNING,
                    "Can not write performance data, changes will not be saved.");
    }
}

PerformanceManager *PerformanceManager::getInstance()
{
    return instance;
}

bool PerformanceManager::getVirtualizationSupportStatus()
{
    return (VBoxWrapperClient::getInstance()->message()->message(L"get vmxSupport") == L"true");
}
