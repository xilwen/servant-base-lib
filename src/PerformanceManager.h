#ifndef TESTEXE_PERFORMANCEMANAGER_H
#define TESTEXE_PERFORMANCEMANAGER_H


class PerformanceManager
{
public:
    PerformanceManager();

    virtual ~PerformanceManager();

    int getProcessorRate();

    int getMemoryRate();

    int getDiskRate();

    bool getVirtualizationSupportStatus();

    void setProcessorRate(int processorRate);

    void setMemoryRate(int memoryRate);

    void setDiskRate(int diskRate);

    static PerformanceManager *getInstance();

private:
    int processorRate;
    int memoryRate;
    int diskRate;
    static PerformanceManager *instance;
};


#endif //TESTEXE_PERFORMANCEMANAGER_H
