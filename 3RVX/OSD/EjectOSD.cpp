// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "EjectOSD.h"

#include <Dbt.h>

#include "../HotkeyInfo.h"
#include "../Monitor.h"
#include "../NotifyIcon.h"
#include "../Skin/OSDComponent.h"
#include "../Skin/Skin.h"
#include "../Skin/SkinManager.h"

EjectOSD::EjectOSD() :
OSD(L"3RVX-EjectDispatcher"),
_mWnd(L"3RVX-EjectOSD", L"3RVX-EjectOSD") {

    SkinManager *skin = SkinManager::Instance();
    _mWnd.BackgroundImage(skin->EjectOSD()->background);
    _mWnd.EnableGlass(skin->EjectOSD()->mask);
    _mWnd.Update();

    OSD::InitMeterWnd(_mWnd);

    //TODO: this needs to be differentiated from the volume OSD.
    if (_settings->NotifyIconEnabled()) {
        _iconImages = skin->VolumeIconset();
        if (_iconImages.size() > 0) {
            _icon = new NotifyIcon(Window::Handle(), L"Eject", _iconImages[0]);
        }
    }

    _menu = CreatePopupMenu();
    //_menuThread = std::thread();
    UpdateDriveMenu();

}

void EjectOSD::UpdateDriveMenu() {
    DWORD drives = GetLogicalDrives();
    /* Get the most significant bit of the drive bitset */
    DWORD msb = (DWORD) log2(drives);
    for (DWORD i = 0; i < msb; ++i, drives >>= 1) {
        if (drives & 0x1) {
            wchar_t letter = (wchar_t) i + 65;
            wchar_t drivePath[] = L" :\\";
            drivePath[0] = letter;
            UINT type = GetDriveType(drivePath);
//            if (type != DRIVE_FIXED) {
//                continue;
//            }
//            if (type == DRIVE_REMOVABLE || type == DRIVE_CDROM) {
                //TODO: certain usb-attached disks will return DRIVE_FIXED here.
                //need to verify with IOCTL
                wchar_t driveName[256] = { 0 };
                int result = GetVolumeInformation(drivePath, driveName, 256,
                    NULL, NULL, NULL, NULL, NULL);
                CLOG(L"Drive: %c - %s [%d]", letter, driveName, result);
                HANDLE dev = CreateFile(
                    DriveFileName(letter).c_str(),
                    FILE_READ_ATTRIBUTES,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    NULL, OPEN_EXISTING, 0, NULL);
                if (dev == INVALID_HANDLE_VALUE) {
                    CLOG(L"Failed to get device handle");
                }


                STORAGE_PROPERTY_QUERY spq;
                spq.AdditionalParameters[0] = 0;
                spq.PropertyId = StorageDeviceProperty;
                spq.QueryType = PropertyStandardQuery;
                STORAGE_DEVICE_DESCRIPTOR sdd = { 0 };
                BYTE bbuf[4096];
                DWORD bytesOut;
                BOOL iores = DeviceIoControl(
                    dev,
                    IOCTL_STORAGE_QUERY_PROPERTY,
                    &spq,
                    sizeof(spq),
                    &sdd,
                    sizeof(sdd),
                    &bytesOut,
                    NULL);
                if (!iores) {
                    Logger::LogLastError();
                }
                CLOG(L"out: %d", bytesOut);
                if (sdd.RemovableMedia) {
                    CLOG(L"This is removable");
                }

                STORAGE_HOTPLUG_INFO shi = { 0 };
                iores = DeviceIoControl(
                    dev,
                    IOCTL_STORAGE_GET_HOTPLUG_INFO,
                    NULL,
                    0,
                    &shi,
                    sizeof(shi),
                    &bytesOut,
                    NULL);
                if (!iores) {
                    Logger::LogLastError();
                }
                CLOG(L"%d %d %d %d", bytesOut, shi.DeviceHotplug, shi.MediaHotplug, shi.MediaHotplug);

                CloseHandle(dev);



//            }
        }
    }
}

void EjectOSD::EjectDrive(std::wstring driveLetter) {
    std::wstring name = L"\\\\.\\" + driveLetter + L":";
    CLOG(L"Ejecting %s", name.c_str());

    HANDLE dev = CreateFile(
        name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        NULL,
        NULL);

    if (dev == INVALID_HANDLE_VALUE) {
        CLOG(L"Failed to get device handle");
        return;
    }

    DWORD bytesReturned = 0;

    DeviceIoControl(dev, FSCTL_LOCK_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    PREVENT_MEDIA_REMOVAL pmr = { 0 };
    pmr.PreventMediaRemoval = FALSE;
    DeviceIoControl(dev, IOCTL_STORAGE_MEDIA_REMOVAL,
        &pmr, sizeof(PREVENT_MEDIA_REMOVAL), NULL, 0, &bytesReturned, NULL);

    DeviceIoControl(dev, FSCTL_DISMOUNT_VOLUME,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    BOOL ejected = FALSE;
    ejected = DeviceIoControl(dev, IOCTL_STORAGE_EJECT_MEDIA,
        NULL, NULL, NULL, NULL, &bytesReturned, NULL);

    if (ejected != FALSE) {
        std::wstring rootPath = driveLetter + L":\\";
        if (GetDriveType(rootPath.c_str()) != DRIVE_CDROM) {
            int driveBit = (int) pow(2, (driveLetter.at(0) - 65));
            _ignoreDrives |= driveBit;
            CLOG(L"Added drive bit %d to ignore list", driveBit);
        }
        if (OSD::Enabled()) {
            HideOthers(Eject);
            _mWnd.Show();
        }
    }

    CloseHandle(dev);
}

void EjectOSD::Hide() {
    _mWnd.Hide(false);
}

void EjectOSD::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {
    case HotkeyInfo::EjectDrive:
        if (hki.args.size() > 0) {
            EjectDrive(hki.args[0]);
        }
        break;

    case HotkeyInfo::EjectLastDisk:
        if (_latestDrive != 0) {
            wchar_t letter = MaskToDriveLetter(_latestDrive);
            std::wstring lStr(1, letter);
            EjectDrive(lStr);
            _latestDrive = 0;
        }

        break;
    }
}

LRESULT
EjectOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEREMOVECOMPLETE) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;

            DWORD driveMask = lpdbv->dbcv_unitmask;
            CLOG(L"Eject notification received for drive %c:",
                MaskToDriveLetter(driveMask));

            if (driveMask & _ignoreDrives) {
                CLOG(L"Drive already ejected by a hotkey; not displaying OSD.");
                _ignoreDrives ^= driveMask;
            } else if (OSD::Enabled()) {
                HideOthers(Eject);
                _mWnd.Show();
            }

            if (_latestDrive == driveMask) {
                _latestDrive = 0;
            }
        }
    }

    if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEARRIVAL) {
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR) lParam;
        if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME) {
            PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME) lpdb;
            
            if (lpdbv->dbcv_flags != DBTF_NET) {
                _latestDrive = lpdbv->dbcv_unitmask;

                CLOG(L"Media inserted in drive %c:",
                    MaskToDriveLetter(_latestDrive));
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD EjectOSD::DriveLetterToMask(wchar_t letter) {
    if (letter < 65 || letter > 90) {
        return 0;
    }

    return (DWORD) pow(2, (letter - 65));
}

wchar_t EjectOSD::MaskToDriveLetter(DWORD mask) {
    return (wchar_t) (log2(mask) + 65);
}

std::wstring EjectOSD::DriveFileName(wchar_t driveLetter) {
    return DriveFileName(std::wstring(1, driveLetter));
}

std::wstring EjectOSD::DriveFileName(std::wstring &driveLetter) {
    return L"\\\\.\\" + driveLetter + L":";
}

void EjectOSD::OnDisplayChange() {
    InitMeterWnd(_mWnd);
}
