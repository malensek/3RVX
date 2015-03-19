#pragma once

#include <vector>

#include "..\Controllers\Volume\CoreAudio.h"
#include "..\Controllers\Volume\VolumeController.h"
#include "..\MeterWnd\Animations\FadeOut.h"
#include "..\MeterWnd\MeterCallbackReceiver.h"
#include "..\MeterWnd\MeterWnd.h"
#include "..\NotifyIcon.h"
#include "OSD.h"

class CallbackMeter;
class Skin;
class SoundPlayer;
class VolumeSlider;

class VolumeOSD : public OSD, MeterCallbackReceiver {
public:
    VolumeOSD();
    ~VolumeOSD();

    void Hide();
    void HideIcon();

    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    CoreAudio *_volumeCtrl;
    float _defaultIncrement;
    float _lastVolume;
    bool _muted;

    MeterWnd _mWnd;
    CallbackMeter *_callbackMeter;
    MeterWnd _muteWnd;
    VolumeSlider *_volumeSlider;

    NotifyIcon *_icon;
    std::vector<HICON> _iconImages;
    int _lastIcon;

    HMENU _menu;
    HMENU _deviceMenu;
    UINT _menuFlags;
    std::vector<VolumeController::DeviceInfo> _deviceList;
    std::wstring _selectedDevice;
    std::wstring _selectedDesc;

    bool _sounds;
    SoundPlayer *_soundPlayer;

    void LoadSkin();
    void MeterLevels(float value);
    virtual void MeterChangeCallback(int units);
    void UpdateIcon();
    void UpdateIconImage();
    void UpdateIconTip();
    void UpdateDeviceMenu();
    void UnMute();

    virtual void UpdateWindowPositions(std::vector<Monitor> &monitors);

    virtual LRESULT
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};