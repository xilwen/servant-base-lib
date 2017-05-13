#ifndef SERVANTBASE_VBOXWRAPPERHOLDER_H
#define SERVANTBASE_VBOXWRAPPERHOLDER_H

#include <string>
#include <windows.h>

class VMWrapperHolder
{
public:
    VMWrapperHolder();
    ~VMWrapperHolder();
    void start();
    void stop();
    bool isRunning();
    static VMWrapperHolder* getInstance();
private:
    const std::string serviceName = "SERVANTVBoxWrapper";
    std::string className = "VMWrapperHolder";
    static VMWrapperHolder* instance;
    SC_HANDLE serviceControlManager = nullptr;
    SC_HANDLE service = nullptr;
    SERVICE_STATUS_PROCESS serviceStatusProcess;
    int stopwatch = 0;

    void openService();
    bool isStarting();
    bool isStopped();

    void queryServiceStatus();
};


#endif //TESTEXE_VBOXWRAPPERHOLDER_H
