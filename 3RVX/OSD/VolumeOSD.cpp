// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "VolumeOSD.h"

#pragma comment(lib, "Wtsapi32.lib")

#include <string>
#include <Wtsapi32.h>

#include "../3RVX.h"
#include "../Controllers/Volume/CurveTransformer.h"
#include "../Controllers/Volume/VolumeLimiter.h"
#include "../HotkeyInfo.h"
#include "../LanguageTranslator.h"
#include "../MeterWnd/LayeredWnd.h"
#include "../MeterWnd/Meters/CallbackMeter.h"
#include "../Monitor.h"
#include "../Skin/OSDComponent.h"
#include "../Skin/Skin.h"
#include "../Skin/SkinManager.h"
#include "../Slider/VolumeSlider.h"
#include "../SoundPlayer.h"

VolumeOSD::VolumeOSD() :
OSD(L"3RVX-VolumeDispatcher"),
_mWnd(L"3RVX-VolumeOSD", L"3RVX-VolumeOSD"),
_muteWnd(L"3RVX-MuteOSD", L"3RVX-MuteOSD") {

    LoadSkin();

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(Window::Handle());
    std::wstring device = _settings->AudioDeviceID();
    _volumeCtrl->Init(device);
    _selectedDesc = _volumeCtrl->DeviceDesc();

    int curveAdjust = _settings->VolumeCurveAdjustment();
    if (curveAdjust > 0) {
        CLOG(L"Using curve adjust factor: %d", curveAdjust);
        CurveTransformer *ct = new CurveTransformer((float) curveAdjust);
        _volumeCtrl->AddTransformation(ct);
        _volumeTransformations.push_back(ct);
    }

    float volumeLimiter = _settings->VolumeLimiter();
    if (volumeLimiter > 0.0f && volumeLimiter < 1.0f) {
        CLOG(L"Using volume limiter: %f", volumeLimiter);
        VolumeLimiter *vl = new VolumeLimiter(volumeLimiter);
        _volumeCtrl->AddTransformation(vl);
        _volumeTransformations.push_back(vl);
    }

    /* Set up volume state variables */
    _lastVolume = _volumeCtrl->Volume();
    _muted = _volumeCtrl->Muted();

    /* Create the slider */
    _volumeSlider = new VolumeSlider(*_volumeCtrl);

    /* Set up context menu */
    if (_settings->NotifyIconEnabled()) {
        LanguageTranslator *translator = _settings->Translator();
        _menuSetStr = translator->Translate(_menuSetStr);
        _menuDevStr = translator->Translate(_menuDevStr);
        _menuMixerStr = translator->Translate(_menuMixerStr);
        _menuExitStr = translator->Translate(_menuExitStr);
        _iconMuteStr = translator->Translate(_iconMuteStr);

        _menu = CreatePopupMenu();
        _deviceMenu = CreatePopupMenu();

        InsertMenu(_menu, -1, MF_ENABLED, MENU_SETTINGS, _menuSetStr.c_str());
        InsertMenu(_menu, -1, MF_POPUP, UINT(_deviceMenu), _menuDevStr.c_str());
        InsertMenu(_menu, -1, MF_ENABLED, MENU_MIXER, _menuMixerStr.c_str());
        InsertMenu(_menu, -1, MF_ENABLED, MENU_EXIT, _menuExitStr.c_str());

        _menuFlags = TPM_RIGHTBUTTON;
        if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
            _menuFlags |= TPM_RIGHTALIGN;
        } else {
            _menuFlags |= TPM_LEFTALIGN;
        }

        UpdateDeviceMenu();
    }

    UpdateIcon();
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);

    if (_settings->MuteOnLock()) {
        /* If muting volume on lock is enabled, register for notifications. */
        _monitorSession = true;
        WTSRegisterSessionNotification(
            Window::Handle(), NOTIFY_FOR_THIS_SESSION);
    }

    if (_settings->ShowOnStartup()) {
        SendMessage(Window::Handle(), VolumeController::MSG_VOL_CHNG,
            NULL, (LPARAM) 1);
    }
}

VolumeOSD::~VolumeOSD() {
    WTSUnRegisterSessionNotification(Window::Handle());
    DestroyMenu(_deviceMenu);
    DestroyMenu(_menu);
    delete _icon;
    delete _volumeSlider;
    delete _callbackMeter;
    for (VolumeTransformation *trans : _volumeTransformations) {
        delete trans;
    }
    _volumeTransformations.clear();
    _volumeCtrl->Dispose();
}

void VolumeOSD::UpdateDeviceMenu() {
    if (_menu == NULL || _deviceMenu == NULL) {
        return;
    }

    /* Remove any devices currently in the menu first */
    for (unsigned int i = 0; i < _deviceList.size(); ++i) {
        RemoveMenu(_deviceMenu, 0, MF_BYPOSITION);
    }
    _deviceList.clear();

    std::vector<VolumeController::DeviceInfo> devices
        = _volumeCtrl->ListDevices();
    std::wstring currentDeviceId = _volumeCtrl->DeviceId();

    /* Disable/enable the menu depending on whether devices are available */
    EnableMenuItem(_menu, UINT(_deviceMenu),
        devices.size() == 0 ? MF_GRAYED : MF_ENABLED);

    int menuItem = MENU_DEVICE;
    for (VolumeController::DeviceInfo device : devices) {
        unsigned int flags = MF_ENABLED;
        if (currentDeviceId == device.id) {
            flags |= MF_CHECKED;
        }

        InsertMenu(_deviceMenu, -1, flags, menuItem++, device.name.c_str());
        _deviceList.push_back(device);
    }
}

void VolumeOSD::LoadSkin() {
    SkinManager *skin = SkinManager::Instance();

    /* Volume OSD */
    OSDComponent *volumeOSD = skin->VolumeOSD();
    _mWnd.BackgroundImage(volumeOSD->background);
    _mWnd.EnableGlass(volumeOSD->mask);
    for (Meter *m : volumeOSD->meters) {
        _mWnd.AddMeter(m);
    }

    /* Add a callback meter with the default volume increment for sounds */
    _callbackMeter = new CallbackMeter(volumeOSD->defaultUnits, *this);
    _mWnd.AddMeter(_callbackMeter);

    /* Default volume increment */
    _defaultIncrement = (float) (10000 / volumeOSD->defaultUnits) / 10000.0f;
    CLOG(L"Default volume increment: %f", _defaultIncrement);

    _mWnd.Update();

    /* Mute OSD */
    _muteWnd.BackgroundImage(skin->MuteOSD()->background);
    _muteWnd.EnableGlass(skin->MuteOSD()->mask);
    _muteWnd.Update();

    /* Now that everything is set up, initialize the meter windows. */
    OSD::InitMeterWnd(_mWnd);
    OSD::InitMeterWnd(_muteWnd);

    /* Set up notification icon */
    if (_settings->NotifyIconEnabled()) {
        _iconImages = skin->VolumeIconset();
        if (_iconImages.size() > 0) {
            _icon = new NotifyIcon(Window::Handle(), L"3RVX", _iconImages[0]);
        }
    }

    /* Enable sound effects, if any */
    if (_settings->SoundEffectsEnabled()) {
        _soundPlayer = volumeOSD->sound;
    }
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
}

void VolumeOSD::MeterChangeCallback(int units) {
    /* This method is called each time the callback meter changes by at least
     * 1 volume unit (as defined by the skin's default unit amount). The
     * callback meter is also used for incrementing/decrementing the volume by
     * skin units. */
}

void VolumeOSD::Hide() {
    _mWnd.Hide(false);
    _muteWnd.Hide(false);
}

void VolumeOSD::Show(bool mute) {
    if (OSD::Enabled() == false) {
        return;
    }

    if (mute) {
        _muteWnd.Show();
        _mWnd.Hide(false);
    } else {
        _mWnd.Show();
        _muteWnd.Hide(false);
    }
}

void VolumeOSD::HideIcon() {
    delete _icon;
}

void VolumeOSD::UpdateIcon() {
    UpdateIconImage();
    UpdateIconTip();
}

void VolumeOSD::UpdateIconImage() {
    if (_icon == NULL) {
        return;
    }

    int icon = 0;
    if (_volumeCtrl->Muted() == false) {
        size_t vUnits = _iconImages.size() - 1;
        icon = (int) ceil(_volumeCtrl->Volume() * vUnits);
    }

    if (icon != _lastIcon) {
        _icon->UpdateIcon(_iconImages[icon]);
        _lastIcon = icon;
    }
}

void VolumeOSD::UpdateIconTip() {
    if (_icon == NULL) {
        return;
    }

    if (_volumeCtrl->Muted()) {
        if (_volumeCtrl->DeviceEnabled()) {
            _icon->UpdateToolTip(_selectedDesc + L": " + _iconMuteStr);
        } else {
            _icon->UpdateToolTip(L"No audio device detected");
        }
    } else {
        float v = _volumeCtrl->Volume();
        std::wstring perc = std::to_wstring((int) (v * 100.0f));
        std::wstring level = _selectedDesc + L": " + perc + L"%";
        _icon->UpdateToolTip(level);
    }
}

void VolumeOSD::UnMute() {
    if (_volumeCtrl->Muted() == true) {
        _volumeCtrl->Muted(false);
    }
}

void VolumeOSD::ProcessHotkeys(HotkeyInfo &hki) {
    switch (hki.action) {
    case HotkeyInfo::IncreaseVolume:
    case HotkeyInfo::DecreaseVolume:
        UnMute();
        ProcessVolumeHotkeys(hki);
        break;

    case HotkeyInfo::SetVolume: {
        HotkeyInfo::VolumeKeyArgTypes type = HotkeyInfo::VolumeArgType(hki);
        if (type == HotkeyInfo::VolumeKeyArgTypes::NoArgs) {
            return;
        } else if (type == HotkeyInfo::VolumeKeyArgTypes::Units) {
            int numUnits = hki.ArgToInt(0);
            _volumeCtrl->Volume(numUnits * _defaultIncrement);
        } else if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
            double perc = hki.ArgToDouble(0);
            _volumeCtrl->Volume((float) perc);
        }
    }

        SendMessage(Window::Handle(), VolumeController::MSG_VOL_CHNG,
            NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::Mute:
        _volumeCtrl->ToggleMute();
        SendMessage(Window::Handle(), VolumeController::MSG_VOL_CHNG,
            NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::VolumeSlider:
        if (_volumeSlider->Visible()) {
            /* If the slider is already visible, user must want to close it. */
            _volumeSlider->Hide();
        } else {
            SendMessage(Window::Handle(), MSG_NOTIFYICON, NULL, WM_LBUTTONUP);
        }
        break;
    }
}

void VolumeOSD::ProcessVolumeHotkeys(HotkeyInfo &hki) {
    float currentVol = _volumeCtrl->Volume();
    HotkeyInfo::VolumeKeyArgTypes type = HotkeyInfo::VolumeArgType(hki);

    if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
        /* Deal with percentage-based amounts */
        float amount = ((float) hki.ArgToDouble(0) / 100.0f);
        if (hki.action == HotkeyInfo::HotkeyActions::DecreaseVolume) {
            amount = -amount;
        }
        _volumeCtrl->Volume(currentVol + amount);
    } else {
        /* Unit-based amounts */
        int unitIncrement = 1;
        int currentUnit = _callbackMeter->CalcUnits();
        if (currentVol <= 0.000001f) {
            currentUnit = 0;
        }

        if (hki.action == HotkeyInfo::DecreaseVolume) {
            unitIncrement = -1;
        }

        if (type == HotkeyInfo::VolumeKeyArgTypes::Units) {
            unitIncrement *= hki.ArgToInt(0);
        }

        _volumeCtrl->Volume(
            (float) (currentUnit + unitIncrement) * _defaultIncrement);
    }

    /* Tell 3RVX that we changed the volume */
    SendMessage(Window::Handle(), VolumeController::MSG_VOL_CHNG,
        NULL, (LPARAM) 1);
}

void VolumeOSD::UpdateVolumeState() {
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);
    UpdateIcon();
}

void VolumeOSD::OnDeviceChange() {
    CLOG(L"Volume device change detected.");
    if (_selectedDevice == L"") {
        _volumeCtrl->SelectDefaultDevice();
    } else {
        HRESULT hr = _volumeCtrl->SelectDevice(_selectedDevice);
        if (FAILED(hr)) {
            _volumeCtrl->SelectDefaultDevice();
        }
    }
    _selectedDesc = _volumeCtrl->DeviceDesc();
    UpdateDeviceMenu();
    UpdateVolumeState();
}

void VolumeOSD::OnDisplayChange() {
    InitMeterWnd(_mWnd);
    InitMeterWnd(_muteWnd);
}

void VolumeOSD::OnMenuEvent(WPARAM wParam) {
    int menuItem = LOWORD(wParam);
    switch (menuItem) {
    case MENU_SETTINGS:
        Settings::LaunchSettingsApp();
        break;

    case MENU_MIXER: {
        CLOG(L"Menu: Mixer");
        HINSTANCE code = ShellExecute(NULL, L"open", L"sndvol",
            NULL, NULL, SW_SHOWNORMAL);
        break;
    }

    case MENU_EXIT:
        CLOG(L"Menu: Exit: %d", (int) _masterWnd);
        SendMessage(_masterWnd, WM_CLOSE, NULL, NULL);
        break;
    }

    /* Device menu items */
    if ((menuItem & MENU_DEVICE) > 0) {
        int device = menuItem & 0x0FFF;
        VolumeController::DeviceInfo selectedDev = _deviceList[device];
        if (selectedDev.id != _volumeCtrl->DeviceId()) {
            /* A different device has been selected */
            CLOG(L"Changing to volume device: %s",
                selectedDev.name.c_str());
            _volumeCtrl->SelectDevice(selectedDev.id);
            UpdateDeviceMenu();
            UpdateVolumeState();
        }
    }
}

void VolumeOSD::OnNotifyIconEvent(HWND hWnd, LPARAM lParam) {
    if (lParam == WM_LBUTTONUP) {
        if (_volumeCtrl->DeviceEnabled()) {
            _volumeSlider->MeterLevels(_volumeCtrl->Volume());
            _volumeSlider->Show();
        }
    } else if (lParam == WM_RBUTTONUP) {
        POINT p;
        GetCursorPos(&p);
        SetForegroundWindow(hWnd);
        TrackPopupMenuEx(_menu, _menuFlags, p.x, p.y,
            Window::Handle(), NULL);
        PostMessage(hWnd, WM_NULL, 0, 0);
    }
}

void VolumeOSD::OnSessionChange(WPARAM wParam) {
    if (_monitorSession == false) {
        return;
    }

    if (wParam == WTS_SESSION_LOCK) {
        CLOG(L"Muting on session lock");
        _unlockUnmute = true;
        _volumeCtrl->Muted(true);
    } else if (wParam == WTS_SESSION_UNLOCK && _unlockUnmute) {
        CLOG(L"Unmuting after session lock");
        _unlockUnmute = false;
        _volumeCtrl->Muted(false);
    }
}

void VolumeOSD::OnVolumeChange(HWND hWnd, WPARAM wParam, LPARAM lParam) {
    float v = _volumeCtrl->Volume();
    bool muteState = _volumeCtrl->Muted();

    _volumeSlider->MeterLevels(v);
    UpdateIcon();

    if (wParam > 0) {
        /* We manually post a MSG_VOL_CHNG when modifying the volume with
         * hotkeys, so this CoreAudio-generated event can be ignored
         * by the OSD. */
        CLOG(L"Ignoring volume change notification generated by 3RVX");
        return;
    }

    CLOG(L"Volume change notification:\nNew level: %f\nPrevious: %f",
        v, _lastVolume);
    if (lParam == 0 && (muteState == _muted)) {
        if (abs(v - _lastVolume) < 0.0001f) {
            CLOG(L"No change in volume detected; ignoring event.");
            return;
        }
    }
    _lastVolume = v;
    _muted = muteState;

    if (_volumeSlider->Visible() == false) {
        if (_volumeCtrl->Muted() || v == 0.0f) {
            Show(true);
        } else {
            MeterLevels(v);
            Show();

            if (_soundPlayer) {
                _soundPlayer->Play();
            }
        }
        HideOthers(Volume);
    }
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case VolumeController::MSG_VOL_CHNG:
        OnVolumeChange(hWnd, wParam, lParam);
        break;

    case VolumeController::MSG_VOL_DEVCHNG:
        OnDeviceChange();
        break;

    case MSG_NOTIFYICON:
        OnNotifyIconEvent(hWnd, lParam);
        break;

    case WM_COMMAND:
        OnMenuEvent(wParam);
        break;

    case WM_WTSSESSION_CHANGE:
        OnSessionChange(wParam);
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
