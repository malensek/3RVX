#pragma once

#include <vector>

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Controllers\Volume\VolumeController.h"
#include "..\MeterWnd\Animations\FadeOut.h"
#include "..\MeterWnd\MeterWnd.h"
#include "..\NotifyIcon.h"
#include "OSD.h"

class Skin;

class VolumeOSD : public OSD {
public:
    VolumeOSD(HINSTANCE hInstance, Settings &settings);

    void Hide();
    void HideIcon();

private:
    MeterWnd _mWnd;
    NotifyIcon *_icon;
    HMENU _menu;
    HMENU _deviceMenu;
    UINT _menuFlags;
    CoreAudio *_volumeCtrl;
    std::vector<VolumeController::DeviceInfo> _deviceList;
    std::wstring _selectedDevice;
    std::wstring _selectedDesc;
    MeterWnd _muteWnd;
    Gdiplus::Bitmap *_muteBg;

    void LoadSkin(std::wstring skinXML);
    void MeterLevels(float value);
    void UpdateIconTip();
    void UpdateDeviceMenu();

    virtual LRESULT
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};