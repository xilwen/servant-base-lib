#include "logger.h"
#include <iostream>

logger::logger()
{
}


logger::~logger()
{
}

void logger::log(std::string className, std::string func, InfoLevel level, std::string string)
{
    printBasicInfo(level);
    auto newLineCharPos(string.find("\n"));
    if(newLineCharPos != std::string::npos)
    {
        string.replace(newLineCharPos, 1, " ");
    }
    std::cout << "] [" << className << ":" << func << "] " << string << std::endl;
}

void logger::log(std::string className, std::string func, InfoLevel level, std::wstring wstring)
{
    printBasicInfo(level);
    std::cout << "] [" << className << ":" << func << "] ";
    std::wcout << wstring << std::endl;
}

void logger::printBasicInfo(const InfoLevel &level)
{
    std::cout << "[";
    switch (level)
    {
    case INFO:
        std::cout << "Info";
        break;
    case WARNING:
        std::cout << "Warn";
        break;
    case ERR:
        std::cout << "Errr";
        break;
    default:
        std::cout << "????";
    }
}
