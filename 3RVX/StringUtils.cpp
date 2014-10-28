#include "StringUtils.h"

std::wstring StringUtils::Widen(const char *str) {
    std::string sstr(str);
    return Widen(sstr);
}

std::wstring StringUtils::Widen(std::string &str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(),
        NULL, 0);
    std::wstring buf(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &buf[0], size);
    return buf;
}

std::string StringUtils::Narrow(std::wstring &str) {
    int size = WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(),
        NULL, 0, NULL, NULL);
    std::string buf(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, &str[0], (int) str.size(),
        &buf[0], size, NULL, NULL);
    return buf;
}