#include <stdexcept>
#include <thread>
#include "programHolder.hpp"
#include "logger.hpp"

programHolder::programHolder()
{
    logger::log(className, __func__, InfoLevel::INFO, className + " initializing...");
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = true;
    securityAttributes.lpSecurityDescriptor = nullptr;
    //TODO Refactor with exception handling(CreatePipe/SetHandle... both return something)
    CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, 0);
    SetHandleInformation(childStdOutRead, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0);
    SetHandleInformation(childStdInWrite, HANDLE_FLAG_INHERIT, 0);
    logger::log(className, __func__, InfoLevel::INFO, className + " initialized.");
}

programHolder::~programHolder()
{
    //TODO Close Child Handle First
    CloseHandle(childStdInWrite);
    CloseHandle(childStdOutRead);
    logger::log(className, __func__, InfoLevel::INFO, className + " destructed.");
}

void programHolder::setCmdLine(const std::string &cmdLine)
{
    this->cmdLine = cmdLine;
}

void programHolder::run()
{
    if (isRunning())
    {
        logger::log(className, __func__, InfoLevel::INFO, cmdLine + " is already running! Rejected.");
        return;
    }
    logger::log(className, __func__, InfoLevel::INFO, "Trying to launch " + cmdLine);
    STARTUPINFO startupinfo;
    int succeed(false);

    ZeroMemory(&processInformation, sizeof(processInformation));
    ZeroMemory(&startupinfo, sizeof(startupinfo));
    startupinfo.cb = sizeof(STARTUPINFO);
    startupinfo.hStdError = childStdOutWrite;
    startupinfo.hStdOutput = childStdOutWrite;
    startupinfo.hStdInput = childStdInRead;
    startupinfo.dwFlags |= STARTF_USESTDHANDLES;

    succeed = CreateProcess(nullptr, LPSTR(cmdLine.c_str()), nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr,
                            &startupinfo,
                            &processInformation);

    if (!succeed)
        throw std::runtime_error("Create Process Failed");
    running = true;
    std::thread stdOutReadThread(&programHolder::stdOutPipeRunner, this);
    stdOutReadThread.detach();
}

std::string *programHolder::getStdOut()
{
    return &stdOut;
}

void programHolder::appendToStdIn(std::string in)
{
    //TODO wait for implement
    throw std::runtime_error("appendToStdIn has not been implemented");
}

std::string programHolder::getCmdLine()
{
    return cmdLine;
}

bool programHolder::isRunning()
{
    return running;
}

void programHolder::stdOutPipeRunner()
{
    DWORD numberOfBytesRead;
    char buffer[BUFFERSIZE];
    int succeed(false);
    while (true)
    {
        succeed = ReadFile(childStdOutRead, buffer, BUFFERSIZE, &numberOfBytesRead, nullptr);
        stdOut += std::string(buffer);
        if (!succeed || numberOfBytesRead == 0)
        {
            break;
        }
    }
    running = false;
}

void programHolder::stdInPipeRunner()
{
    //TODO wait for implement
}
