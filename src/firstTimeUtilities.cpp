#include <cmath>
#include <chrono>
#include "FirstTimeUtilities.hpp"
#include "PerformanceManager.hpp"
#include "VBoxWrapperClient.hpp"

FirstTimeUtilities::FirstTimeUtilities()
{

}

FirstTimeUtilities::~FirstTimeUtilities()
{

}

void FirstTimeUtilities::runProcessorTest()
{
    //euler's Pi algorithm, in Core i3-4130 it takes about 7 seconds
    //TODO timer start
    auto a1S = std::chrono::high_resolution_clock::now();
    double ret = 0.0;
    for (long i = 1; i <= pow(15, 7); ++i)
    {
        ret += 1.0 / static_cast<double>(i * i);
    }
    sqrt(ret * 6.0);
    auto a1E = std::chrono::high_resolution_clock::now();
    double time(std::chrono::duration<double, std::milli>(a1E - a1S).count());
    int result(static_cast<int>(time));
    PerformanceManager::getInstance()->setProcessorRate(result);
}

void FirstTimeUtilities::runMemoryTest()
{
    PerformanceManager::getInstance()->setMemoryRate(
            std::stoi(VBoxWrapperClient::getInstance()->message()->message(L"get systemTotalRAM")));
}

void FirstTimeUtilities::runDiskTest()
{
    //TODO implement this
    PerformanceManager::getInstance()->setDiskRate(100);
}
