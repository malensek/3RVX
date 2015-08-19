// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
    void Show(bool mute = false);
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

    SoundPlayer *_soundPlayer;

    void LoadSkin();
    void MeterLevels(float value);
    virtual void MeterChangeCallback(int units);
    void ProcessVolumeHotkeys(HotkeyInfo &hki);
    void UpdateIcon();
    void UpdateIconImage();
    void UpdateIconTip();
    void UpdateDeviceMenu();
    void UnMute();

    /// <summary>
    /// Re-initializes volume state variables (used after a device change
    /// occurs).
    /// </summary>
    void UpdateVolumeState();

    virtual LRESULT
        WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    std::wstring _menuSetStr = L"Settings";
    std::wstring _menuDevStr = L"Audio Device";
    std::wstring _menuMixerStr = L"Mixer";
    std::wstring _menuExitStr = L"Exit 3RVX";

    std::wstring _iconMuteStr = L"Muted";

private:
    static const int MENU_SETTINGS = 0;
    static const int MENU_MIXER = 1;
    static const int MENU_EXIT = 2;
    static const int MENU_DEVICE = 0xF000;

};