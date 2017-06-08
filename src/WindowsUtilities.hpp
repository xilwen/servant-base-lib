#ifndef SERVANTBASE_WINDOWSUTILITIES_HPP
#define SERVANTBASE_WINDOWSUTILITIES_HPP

#include <string>

class WindowsUtilities
{
public:
    static std::wstring getHomePath();
    static void ipconfigAndSave();
    static std::string getIpconfig();
    static void startURI(std::string uri);
};


#endif //SERVANTBASE_WINDOWSUTILITIES_HPP
