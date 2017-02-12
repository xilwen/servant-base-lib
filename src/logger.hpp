#pragma once
#include <string>

enum InfoLevel //Message Output Level
{
    INFO, WARNING, ERR
};

class logger
{
public:
    logger();
    static void log(std::string className, std::string func, InfoLevel level, std::string string);
    static void log(std::string className, std::string func, InfoLevel level, std::wstring wstring);

    ~logger();
private:
    static void printBasicInfo(const InfoLevel &level);
};


