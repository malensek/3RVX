#include "DiskInfo.h"
#include "Logger.h"

DiskInfo::DiskInfo(wchar_t driveLetter) {
    std::wstring driveFileName = DriveFileName(driveLetter);
    _devHandle = CreateFile(
        driveFileName.c_str(),
        FILE_READ_ATTRIBUTES,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (_devHandle == INVALID_HANDLE_VALUE) {
        CLOG(L"Failed to get device handle");
    }

}

std::wstring DiskInfo::DriveFileName(wchar_t &driveLetter) {
    return DriveFileName(std::wstring(1, driveLetter));
}

std::wstring DiskInfo::DriveFileName(std::wstring &driveLetter) {
    return L"\\\\.\\" + driveLetter + L":";
}
