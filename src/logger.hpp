#pragma once

#include <string>
#include <fstream>

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
    static void printBasicInfo(const InfoLevel &level, std::stringstream &stringstream);

    static std::ofstream *openLogFile();

    static void lockWriting();

    static void unlockWriting();

    static std::string toString(const std::wstring &string);

    static std::string logFileName;

    static bool writeLock;

    static void findAndDelete(std::string &timeString, const std::string &toReplace);
};
