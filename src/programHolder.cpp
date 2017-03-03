#include <stdexcept>
#include <thread>
#include "ProgramHolder.hpp"
#include "Logger.hpp"

ProgramHolder::ProgramHolder()
{
    Logger::log(className, __func__, InfoLevel::INFO, className + " initializing...");
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = true;
    securityAttributes.lpSecurityDescriptor = nullptr;
    //TODO Refactor with exception handling(CreatePipe/SetHandle... both return something)
    CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, 0);
    SetHandleInformation(childStdOutRead, HANDLE_FLAG_INHERIT, 0);
    CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0);
    SetHandleInformation(childStdInWrite, HANDLE_FLAG_INHERIT, 0);
    ZeroMemory(&processInformation, sizeof(processInformation));
    Logger::log(className, __func__, InfoLevel::INFO, className + " initialized.");
}

ProgramHolder::~ProgramHolder()
{
    Logger::log(className, __func__, InfoLevel::INFO, className + " destruction started.");
    CloseHandle(childStdInWrite);
    CloseHandle(childStdOutRead);
    CloseHandle(processInformation.hProcess);
    CloseHandle(processInformation.hThread);
    Logger::log(className, __func__, InfoLevel::INFO, className + " destructed.");
}

void ProgramHolder::setCmdLine(const std::string &cmdLine)
{
    this->cmdLine = cmdLine;
}

void ProgramHolder::run()
{
    if (isRunning())
    {
        Logger::log(className, __func__, InfoLevel::INFO, cmdLine + " is already running! Rejected.");
        return;
    }
    Logger::log(className, __func__, InfoLevel::INFO, "Trying to launch " + cmdLine);
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
    std::thread stdOutReadThread(&ProgramHolder::stdOutPipeRunner, this);
    stdOutReadThread.detach();
}

std::string *ProgramHolder::getStdOut()
{
    return &stdOut;
}

void ProgramHolder::appendToStdIn(std::string in)
{
    //TODO wait for implement
    throw std::runtime_error("appendToStdIn has not been implemented");
}

std::string ProgramHolder::getCmdLine()
{
    return cmdLine;
}

bool ProgramHolder::isRunning()
{
    if (processInformation.hProcess == nullptr)
        return false;
    unsigned long exitCode;
    GetExitCodeProcess(processInformation.hProcess, &exitCode);
    return (STILL_ACTIVE == exitCode);
}

void ProgramHolder::stdOutPipeRunner()
{
    DWORD numberOfBytesRead;
    char buffer[BUFFERSIZE];
    int succeed(false);
    while (isRunning())
    {
        succeed = ReadFile(childStdOutRead, buffer, BUFFERSIZE, &numberOfBytesRead, nullptr);
        stdOut += std::string(buffer);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (strstr(buffer, "exit") != nullptr || !succeed || numberOfBytesRead == 0)
        {
            break;
        }
    }
}

void ProgramHolder::stdInPipeRunner()
{
    //TODO wait for implement
}
