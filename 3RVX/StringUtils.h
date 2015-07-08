#pragma once

#include <Windows.h>
#include <string>

class StringUtils {
public:
    static std::wstring Widen(const char *str);
    static std::wstring Widen(std::string &str);
    static std::string Narrow(std::wstring &str);
    /// <summary>
    /// Retrieves the file extension of the given file name. This is done by
    /// locating the substring after the last dot '.' in the file name; the dot
    /// character is not included in the string returned by this function.
    /// </summary>
    static std::wstring FileExtension(const std::wstring &fileName);
};