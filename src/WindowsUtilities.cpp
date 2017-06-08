#include "WindowsUtilities.hpp"
#include "ProfileManager.hpp"
#include <windows.h>
#include <fstream>

std::wstring WindowsUtilities::getHomePath()
{
    auto homePathDataBuffer(LPWSTR(malloc(256 * sizeof(WCHAR)))),
            homeDriveDataBuffer(LPWSTR(malloc(256 * sizeof(WCHAR))));
    GetEnvironmentVariableW(L"HOMEPATH", homePathDataBuffer, 255);
    GetEnvironmentVariableW(L"HOMEDRIVE", homeDriveDataBuffer, 255);
    auto homePath(std::wstring(homeDriveDataBuffer) + std::wstring(homePathDataBuffer));
    return homePath;
}

void WindowsUtilities::ipconfigAndSave()
{
    VBoxWrapperClient::getInstance()->message()->message(L"get ipconfigAndSave");
}

void WindowsUtilities::startURI(std::string uri)
{
    if(uri.empty())
        return;
    std::string command("start " + uri);
    system(command.c_str());
}

std::string WindowsUtilities::getIpconfig()
{
    std::ifstream ifstream;
    std::string string, tempString;
    ifstream.open("C:\\SERVANT\\ipconfig.txt");
    if(ifstream.is_open())
    {
        while(std::getline(ifstream, tempString))
        {
            string += tempString;
            string += '\n';
        }
    }
    return string;
}
