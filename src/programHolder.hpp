#ifndef SERVANTBASE_PROGRAMHANDLER_HPP
#define SERVANTBASE_PROGRAMHANDLER_HPP

//TODO Refactor into programHolderForWindows
#include <windows.h>
#include <string>

class ProgramHolder
{
public:
    ProgramHolder();

    virtual ~ProgramHolder();

    void setCmdLine(const std::string &cmdLine);

    std::string getCmdLine();

    void run();

    bool isRunning();

    std::string *getStdOut();

    void clearStdOut();

private:
    const unsigned long BUFFERSIZE = 4096;
    const std::string className = "ProgramHolder";
    HANDLE childStdInWrite,
            childStdInRead,
            childStdOutWrite,
            childStdOutRead;
    SECURITY_ATTRIBUTES securityAttributes;
    PROCESS_INFORMATION processInformation;
    std::string cmdLine,
            stdOut;
    DWORD exitCode;
    bool running = false;

    void stdOutPipeRunner();
};


#endif //SERVANTBASE_PROGRAMHANDLER_HPP
