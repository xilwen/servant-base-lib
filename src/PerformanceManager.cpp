#include "PerformanceManager.hpp"
#include "Logger.hpp"
#include "ProfileManager.hpp"
#include <json.hpp>

using json = nlohmann::json;
PerformanceManager *PerformanceManager::instance(nullptr);

PerformanceManager::PerformanceManager()
{
    instance = this;
    json j = ProfileManager::getInstance()->getPerformanceJson();
    if (j.find("processorRate") != j.end() && j.find("memoryRate") != j.end() && j.find("diskRate") != j.end())
    {
        setProcessorRate(j.find("processRate").value());
        setMemoryRate(j.find("memoryRate").value());
        setDiskRate(j.find("diskRate").value());
    } else
    {
        Logger::log("PerformanceManager", __func__, InfoLevel::WARNING,
                    "performance profile invalid, using default values");
        setProcessorRate(-1);
        setMemoryRate(-1);
        setDiskRate(-1);
    }
}

PerformanceManager::~PerformanceManager()
{
    writeDataToFile();
    instance = nullptr;
}

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

void PerformanceManager::writeDataToFile()
{
    json j;
    j.emplace("processorRate", getProcessorRate());
    j.emplace("memoryRate", getMemoryRate());
    j.emplace("diskRate", getDiskRate());
    ProfileManager::getInstance()->writePerformanceJson(j);
}

PerformanceManager *PerformanceManager::getInstance()
{
    return instance;
}

bool PerformanceManager::getVirtualizationSupportStatus()
{
    return (VBoxWrapperClient::getInstance()->message()->message(L"get vmxSupport") == L"true");
}
