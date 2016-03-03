#pragma once

#include <Windows.h>

#include <string>

class DriveInfo {
public:
    DriveInfo(wchar_t driveLetter);

    static std::wstring DriveFileName(wchar_t &driveLetter);
    static std::wstring DriveFileName(std::wstring &driveLetter);

private:
    HANDLE _devHandle;
    std::wstring _productId;
    std::wstring _vendorId;
    bool _hasRemovableMedia;

    void PopulateDeviceId();
    void PopulateDeviceInfo();
    void PopulateHotplugInfo();

};