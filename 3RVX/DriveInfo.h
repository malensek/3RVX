// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <string>

class DriveInfo {
public:
    DriveInfo(wchar_t driveLetter);

    const wchar_t DriveLetter();
    bool HasRemovableMedia();
    bool IsHotPluggable();
    const std::wstring &ProductID();
    DWORD SerialNumber();
    const std::wstring &VendorID();
    const std::wstring &VolumeLabel();

    /// <summary>
    /// Given a drive letter, this function retrieves the drive file name
    /// (something of the form \\.\X:), useful for IOCTL operations.
    /// </summary>
    static std::wstring DriveFileName(wchar_t &driveLetter);

    /// <summary>
    /// Given a drive letter, this function retrieves the drive file name
    /// (something of the form \\.\X:), useful for IOCTL operations.
    /// </summary>
    static std::wstring DriveFileName(const std::wstring &driveLetter);

private:
    HANDLE _devHandle;
    DWORD _devId;
    bool _hasRemovableMedia;
    bool _isHotplug;
    wchar_t _letter;
    std::wstring _productId;
    std::wstring _vendorId;
    DWORD _serial;
    std::wstring _volumeName;

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

    /// <summary>
    /// Retrieves the name of the volume associated with this drive as well as
    /// the device serial number.
    /// </summary>
    void PopulateVolumeInfo();
};
