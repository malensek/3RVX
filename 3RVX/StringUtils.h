#pragma once

#include <Windows.h>
#include <string>

class StringUtils {
public:
    static std::wstring Widen(const char *str);
    static std::wstring Widen(std::string &str);
    static std::string Narrow(std::wstring &str);
};