#include <iomanip>
#include <thread>
#include "PackageManager.hpp"
#include "Utilities.hpp"

std::string Utilities::toString(const std::wstring &wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto string(converter.to_bytes(wstring));
    return string;
}