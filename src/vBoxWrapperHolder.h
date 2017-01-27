#ifndef SERVANTBASE_VBOXWRAPPERHOLDER_H
#define SERVANTBASE_VBOXWRAPPERHOLDER_H

#include <string>

class vBoxWrapperHolder
{
public:
    vBoxWrapperHolder();
    vBoxWrapperHolder(std::string path);
    ~vBoxWrapperHolder();
    void start();
    bool isRunning();
    static vBoxWrapperHolder* getInstance();
private:
    std::string vBoxWrapperPath;
    bool wrapperRunning = false;
    void workerThread();
    FILE *consoleOutputPipe;
    static vBoxWrapperHolder* instance;
};


#endif //TESTEXE_VBOXWRAPPERHOLDER_H
