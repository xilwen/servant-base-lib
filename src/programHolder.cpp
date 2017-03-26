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
    {
        Logger::log(className, __func__, InfoLevel::INFO, "Launch failed because " + std::to_string(GetLastError()));
        throw std::runtime_error("Create Process Failed");
        return;
    }
    running = true;
    std::thread stdOutReadThread(&ProgramHolder::stdOutPipeRunner, this);
    stdOutReadThread.detach();
}

std::string *ProgramHolder::getStdOut()
{
    return &stdOut;
}

std::string ProgramHolder::getCmdLine()
{
    return cmdLine;
}

bool ProgramHolder::isRunning()
{
    return running;
}

void ProgramHolder::stdOutPipeRunner()
{
    GetExitCodeProcess(processInformation.hProcess, &exitCode);
    while (exitCode == STILL_ACTIVE)
    {
        DWORD numberOfBytesRead;
        char buffer[BUFFERSIZE] = {0};
        ReadFile(childStdOutRead, buffer, BUFFERSIZE, &numberOfBytesRead, nullptr);
        stdOut += std::string(buffer);
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        GetExitCodeProcess(processInformation.hProcess, &exitCode);
    }
    running = false;
    Logger::log(className, __func__, InfoLevel::INFO, "Process Ended. Return Code = " + std::to_string(exitCode));
}

void ProgramHolder::clearStdOut()
{
    stdOut.clear();
}
