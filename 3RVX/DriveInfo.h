#pragma once

#include <Windows.h>

#include <string>

class DriveInfo {
public:
    DriveInfo(wchar_t driveLetter);

    /// <summary>
    /// Given a drive letter, this function retrieves the drive file name
    /// (something of the form \\.\X:), useful for IOCTL operations.
    /// </summary>
    static std::wstring DriveFileName(wchar_t &driveLetter);

    /// <summary>
    /// Given a drive letter, this function retrieves the drive file name
    /// (something of the form \\.\X:), useful for IOCTL operations.
    /// </summary>
    static std::wstring DriveFileName(std::wstring &driveLetter);

private:
    HANDLE _devHandle;
    DWORD _devId;
    std::wstring _productId;
    std::wstring _vendorId;
    bool _hasRemovableMedia;
    bool _isHotplug;

    /// <summary>Retrieves the numeric identifier for this drive.</summary>
    void PopulateDeviceId();

    /// <summary>
    /// Retrieves information about the specific device: product name,
    /// manufacturer, etc.
    /// </summary>
    void PopulateDeviceInfo();


    /// <summary>
    /// Determines hotplug characteristics for a drive. This includes
    /// whether the drive has removable media.
    /// </summary>
    void PopulateHotplugInfo();

};