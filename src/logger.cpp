#include "Logger.hpp"
#include "PackageManager.hpp"
#include "Utilities.hpp"
#include "ProfileManager.hpp"
#include <thread>
#include <iomanip>

bool Logger::writeLock = false;

std::string Logger::logFileDir;

std::string Logger::logFileName;

Logger::Logger()
{    /*static functions only*/   }

Logger::~Logger()
{}

void Logger::log(std::string className, std::string func, InfoLevel level, std::string string)
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

void Logger::log(std::string className, std::string func, InfoLevel level, std::wstring wstring)
{
    log(className, func, level, Utilities::toString(wstring));
}

void Logger::printBasicInfo(const InfoLevel &level, std::stringstream &stringstream)
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

void Logger::unlockWriting()
{
    writeLock = false;
}

void Logger::lockWriting()
{
    while (writeLock)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    writeLock = true;
}

std::ofstream *Logger::openLogFile()
{
    if (logFileName.empty())
    {
        if (!ProfileManager::getInstance() && logFileDir.empty())
        {
            return nullptr;
        } else if (logFileDir.empty())
        {
            logFileDir = ProfileManager::getInstance()->getUserDataDirString();
        }
        initialLogFileName();
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

void Logger::initialLogFileName()
{
    std::chrono::_V2::system_clock::time_point timePoint = std::chrono::_V2::system_clock::now();
    time_t timeT = std::chrono::_V2::system_clock::to_time_t(timePoint);
    std::__cxx11::string timeString(ctime(&timeT));

    findAndDelete(timeString, " ");
    findAndDelete(timeString, ":");
    findAndDelete(timeString, "\n");

    logFileName = logFileDir + "/" + timeString + ".log";
}

void Logger::findAndDelete(std::string &timeString, const std::string &toReplace)
{
    auto position = timeString.find(toReplace);
    while (position != std::string::npos)
    {
        timeString.replace(position, std::__cxx11::string(toReplace).length(), "");
        position = timeString.find(toReplace);
    }
}

void Logger::setLogFileDir(std::wstring path)
{
    setLogFileDir(Utilities::toString(path));
}

void Logger::setLogFileDir(std::string path)
{
    logFileDir = path;
    if(logFileDir.find("SERVANTConfig") == std::string::npos)
        logFileDir += "/SERVANTConfig";
}
