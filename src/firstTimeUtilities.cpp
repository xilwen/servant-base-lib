#include <cmath>
#include <chrono>
#include "firstTimeUtilities.hpp"
#include "PerformanceManager.hpp"
#include "vBoxWrapperClient.hpp"

firstTimeUtilities::firstTimeUtilities()
{

}

firstTimeUtilities::~firstTimeUtilities()
{

}

void firstTimeUtilities::runProcessorTest()
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

void firstTimeUtilities::runMemoryTest()
{
    PerformanceManager::getInstance()->setMemoryRate(
            std::stoi(vBoxWrapperClient::getInstance()->message()->message(L"get systemTotalRAM")));
}

void firstTimeUtilities::runDiskTest()
{
    //TODO implement this
    PerformanceManager::getInstance()->setDiskRate(100);
}
