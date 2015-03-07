#pragma once

#include <vector>

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Controllers\Volume\VolumeController.h"
#include "..\MeterWnd\Animations\FadeOut.h"
#include "..\MeterWnd\MeterWnd.h"
#include "..\MeterWnd\MeterCallbackReceiver.h"
#include "..\NotifyIcon.h"
#include "..\SoundPlayer.h"
#include "OSD.h"

class Skin;
class VolumeSlider;

class VolumeOSD : public OSD, MeterCallbackReceiver {
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
    float _defaultIncrement;
    bool _sounds;
    SoundPlayer *_soundPlayer;
    float _previousVolume;

    void LoadSkin();
    void MeterLevels(float value);
    virtual void MeterChangeCallback(int units);
    void UpdateIcon();
    void UpdateIconImage();
    void UpdateIconTip();
    void UpdateDeviceMenu();
    void UnMute();

    virtual void UpdateWindowPositions(std::vector<HMONITOR> monitors);

    virtual LRESULT
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};