#pragma once

#include <string>
#include <fstream>

enum InfoLevel //Message Output Level
{
    INFO, WARNING, ERR
};

class Logger
{
public:
    Logger();

    static void log(std::string className, std::string func, InfoLevel level, std::string string);

    static void log(std::string className, std::string func, InfoLevel level, std::wstring wstring);

    static void setLogFileDir(std::wstring path);

    static void setLogFileDir(std::string path);

    ~Logger();

private:
    static void printBasicInfo(const InfoLevel &level, std::stringstream &stringstream);

    static std::ofstream *openLogFile();

    static void lockWriting();

    static void unlockWriting();

    static std::string logFileDir;

    static std::string logFileName;

    static bool writeLock;

    static void findAndDelete(std::string &timeString, const std::string &toReplace);

    static void initialLogFileName();
};
