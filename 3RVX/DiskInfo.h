#pragma once

#include <Windows.h>

#include <string>

class DiskInfo {
public:
    DiskInfo(wchar_t driveLetter);

    static std::wstring DriveFileName(wchar_t &driveLetter);
    static std::wstring DriveFileName(std::wstring &driveLetter);

private:
    HANDLE _devHandle;
};