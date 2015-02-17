#pragma once

#include <vector>

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Controllers\Volume\VolumeController.h"
#include "..\MeterWnd\Animations\FadeOut.h"
#include "..\MeterWnd\MeterWnd.h"
#include "..\NotifyIcon.h"
#include "OSD.h"

class Skin;
class VolumeSlider;

class VolumeOSD : public OSD {
public:
    VolumeOSD();
    ~VolumeOSD();

    void Hide();
    void HideIcon();

    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    MeterWnd _mWnd;
    NotifyIcon *_icon;
    std::vector<HICON> _iconImages;
    int _lastIcon;
    HMENU _menu;
    HMENU _deviceMenu;
    UINT _menuFlags;
    CoreAudio *_volumeCtrl;
    VolumeSlider *_volumeSlider;
    std::vector<VolumeController::DeviceInfo> _deviceList;
    std::wstring _selectedDevice;
    std::wstring _selectedDesc;
    MeterWnd _muteWnd;
    Gdiplus::Bitmap *_muteBg;
    float _defaultIncrement;

    void LoadSkin();
    void MeterLevels(float value);
    void UpdateIcon();
    void UpdateIconImage();
    void UpdateIconTip();
    void UpdateDeviceMenu();
    void UnMute();

    virtual LRESULT
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};