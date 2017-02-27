#include "logger.hpp"
#include "packageManager.hpp"
#include <thread>
#include <iomanip>

bool logger::writeLock = false;

std::string logger::logFileName;

logger::logger()
{    /*static functions only*/   }

logger::~logger()
{}

void logger::log(std::string className, std::string func, InfoLevel level, std::string string)
{
    lockWriting();

    std::ofstream *logFile = openLogFile();
    std::stringstream stringstream;
    printBasicInfo(level, stringstream);
    stringstream << "[";
    stringstream << className << "-" << func << "] " << string << std::endl;

    if (logFile)
    {
        *logFile << stringstream.str();
        logFile->close();
    } else
    {
        std::cout << stringstream.str();
    }

    unlockWriting();

}

void logger::log(std::string className, std::string func, InfoLevel level, std::wstring wstring)
{
    log(className, func, level, toString(wstring));
}

void logger::printBasicInfo(const InfoLevel &level, std::stringstream &stringstream)
{
    stringstream << "[";
    switch (level)
    {
        case INFO:
            stringstream << "INFO";
            break;
        case WARNING:
            stringstream << "WARN";
            break;
        case ERR:
            stringstream << "ERROR";
            break;
        default:
            stringstream << "UNKNOWN";
    }
    stringstream << "]";
}

void logger::unlockWriting()
{
    writeLock = false;
}

void logger::lockWriting()
{
    while (writeLock)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    writeLock = true;
}

std::string logger::toString(const std::wstring &wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto string(converter.to_bytes(wstring));
    return string;
}

std::ofstream *logger::openLogFile()
{
    if (!packageManager::getInstance())
        return nullptr;
    if (logFileName.empty())
    {
        std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();
        std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);
        std::string timeString(std::ctime(&timeT));

        findAndDelete(timeString, " ");
        findAndDelete(timeString, ":");
        findAndDelete(timeString, "\n");

        logFileName = packageManager::getInstance()->getUserDataDir() + "/" + timeString + ".log";
    }
    std::ofstream *ofstream = new std::ofstream(logFileName, std::ios_base::out | std::ios_base::app);
    if (ofstream->is_open())
    {
        return ofstream;
    } else
    {
        std::cout << "Log File can not be wrote.(" << logFileName << ")" << std::strerror(errno) << std::endl;
        return nullptr;
    }
}

void logger::findAndDelete(std::string &timeString, const std::string &toReplace)
{
    auto position = timeString.find(toReplace);
    while (position != std::string::npos)
    {
        timeString.replace(position, std::__cxx11::string(toReplace).length(), "");
        position = timeString.find(toReplace);
    }
}
