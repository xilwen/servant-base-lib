#ifndef SERVANTBASE_VBOXWRAPPERHOLDER_H
#define SERVANTBASE_VBOXWRAPPERHOLDER_H

#include <string>
#include "ProgramHolder.hpp"

class VBoxWrapperHolder
{
public:
    VBoxWrapperHolder();
    VBoxWrapperHolder(std::string path);
    ~VBoxWrapperHolder();
    void start();
    bool isRunning();
    static VBoxWrapperHolder* getInstance();
private:
    std::string vBoxWrapperPath;
    std::string className = "VBoxWrapperHolder";
    bool wrapperRunning = false;
    void workerThread();
    FILE *consoleOutputPipe;
    static VBoxWrapperHolder* instance;
    ProgramHolder* vBoxWrapper = nullptr;
};


#endif //TESTEXE_VBOXWRAPPERHOLDER_H
