#include "WindowsUtilities.hpp"
#include "ProfileManager.hpp"
#include <windows.h>

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
    std::string command("start " + uri);
    system(command.c_str());
}
