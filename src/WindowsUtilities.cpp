#include "WindowsUtilities.hpp"
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
