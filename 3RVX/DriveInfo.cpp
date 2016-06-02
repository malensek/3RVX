// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "DriveInfo.h"

#include "Logger.h"
#include "StringUtils.h"

DriveInfo::DriveInfo(wchar_t driveLetter) :
_letter(driveLetter) {
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
        // TODO perhaps this should throw an error?
        CLOG(L"Failed to get device handle");
        return;
    }

    PopulateDeviceId();
    PopulateDeviceInfo();
    PopulateHotplugInfo();

    CloseHandle(_devHandle);
}

const wchar_t DriveInfo::DriveLetter() {
    return _letter;
}

bool DriveInfo::HasRemovableMedia() {
    return _hasRemovableMedia;
}

bool DriveInfo::IsHotPluggable() {
    return _isHotplug;
}

const std::wstring &DriveInfo::ProductID() {
    return _productId;
}

const std::wstring &DriveInfo::VendorID() {
    return _vendorId;
}

void DriveInfo::PopulateDeviceId() {
    STORAGE_DEVICE_NUMBER sdn = { 0 };
    DWORD bytesOut;

    BOOL result = DeviceIoControl(
        _devHandle,
        IOCTL_STORAGE_GET_DEVICE_NUMBER,
        NULL,
        0,
        &sdn,
        sizeof(sdn),
        &bytesOut,
        NULL);

    if (result == 0) {
        Logger::LogLastError();
        return;
    }

    _devId = sdn.DeviceNumber;
}

void DriveInfo::PopulateDeviceInfo() {
    STORAGE_PROPERTY_QUERY propertyQuery;
    propertyQuery.PropertyId = StorageDeviceProperty;
    propertyQuery.QueryType = PropertyStandardQuery;

    STORAGE_DESCRIPTOR_HEADER storageHeader = { 0 };

    DWORD bytesOut;
    BOOL result;

    result = DeviceIoControl(
        _devHandle,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &propertyQuery,
        sizeof(propertyQuery),
        &storageHeader,
        sizeof(storageHeader),
        &bytesOut,
        NULL);

    if (!result) {
        Logger::LogLastError();
        return;
    }

    unsigned char *descriptorData = new unsigned char[storageHeader.Size];

    result = DeviceIoControl(
        _devHandle,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &propertyQuery,
        sizeof(propertyQuery),
        descriptorData,
        storageHeader.Size,
        &bytesOut,
        NULL);

    if (!result) {
        delete[] descriptorData;
        Logger::LogLastError();
        return;
    }

    STORAGE_DEVICE_DESCRIPTOR *sdd
        = (STORAGE_DEVICE_DESCRIPTOR *) descriptorData;

    if (sdd->ProductIdOffset) {
        _productId = StringUtils::Widen(
            (const char *) ((unsigned char *) sdd + sdd->ProductIdOffset));
    }

    if (sdd->VendorIdOffset) {
        _vendorId = StringUtils::Widen(
            (const char *) ((unsigned char *) sdd + sdd->VendorIdOffset));
    }

    if (sdd->RemovableMedia) {
        _hasRemovableMedia = true;
    }

    CLOG("ProductID: '%s'\nVendor ID: '%s'",
        _productId.c_str(), _vendorId.c_str());

    delete[] descriptorData;
}

void DriveInfo::PopulateHotplugInfo() {
    STORAGE_HOTPLUG_INFO shi = { 0 };
    DWORD bytesOut;

    BOOL result = DeviceIoControl(
        _devHandle,
        IOCTL_STORAGE_GET_HOTPLUG_INFO,
        NULL,
        0,
        &shi,
        sizeof(shi),
        &bytesOut,
        NULL);

    if (result == 0) {
        Logger::LogLastError();
        return;
    }

    if (shi.DeviceHotplug) {
        _isHotplug = true;
    }

    if (shi.MediaRemovable) {
        _hasRemovableMedia = true;
    }
}

std::wstring DriveInfo::DriveFileName(wchar_t &driveLetter) {
    return DriveFileName(std::wstring(1, driveLetter));
}

std::wstring DriveInfo::DriveFileName(const std::wstring &driveLetter) {
    return L"\\\\.\\" + driveLetter + L":";
}

