#include "VolumeOSD.h"

#include "..\SoundPlayer.h"

#include <string>

#include "..\HotkeyInfo.h"
#include "..\MeterWnd\Meters\CallbackMeter.h"
#include "..\Monitor.h"
#include "..\Skin.h"
#include "..\SkinManager.h"
#include "..\Slider\VolumeSlider.h"
#include "..\MeterWnd\LayeredWnd.h"

#define MENU_SETTINGS 0
#define MENU_MIXER 1
#define MENU_EXIT 2
#define MENU_DEVICE 0xF000

VolumeOSD::VolumeOSD() :
OSD(L"3RVX-VolumeDispatcher"),
_mWnd(L"3RVX-VolumeOSD", L"3RVX-VolumeOSD"),
_muteWnd(L"3RVX-MuteOSD", L"3RVX-MuteOSD") {

    LoadSkin();
    Settings *settings = Settings::Instance();

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(_hWnd);
    std::wstring device = settings->AudioDeviceID();
    _volumeCtrl->Init(device);
    _selectedDesc = _volumeCtrl->DeviceDesc();

    /* Set up volume state variables */
    _lastVolume = _volumeCtrl->Volume();
    _muted = _volumeCtrl->Muted();

    if (settings->SoundEffectsEnabled()) {
        _sounds = true;
    }

    /* Create the slider */
    _volumeSlider = new VolumeSlider(*_volumeCtrl);

    /* Set up context menu */
    if (settings->NotifyIconEnabled()) {
        _menu = CreatePopupMenu();
        _deviceMenu = CreatePopupMenu();

        InsertMenu(_menu, -1, MF_ENABLED, MENU_SETTINGS, L"Settings");
        InsertMenu(_menu, -1, MF_POPUP, UINT(_deviceMenu), L"Audio Device");
        InsertMenu(_menu, -1, MF_ENABLED, MENU_MIXER, L"Mixer");
        InsertMenu(_menu, -1, MF_ENABLED, MENU_EXIT, L"Exit");

        _menuFlags = TPM_RIGHTBUTTON;
        if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
            _menuFlags |= TPM_RIGHTALIGN;
        } else {
            _menuFlags |= TPM_LEFTALIGN;
        }

        UpdateDeviceMenu();
    }

    _mWnd.AlwaysOnTop(settings->AlwaysOnTop());
    _mWnd.HideAnimation(settings->HideAnim(), settings->HideSpeed());
    _mWnd.VisibleDuration(settings->HideDelay());

    _muteWnd.AlwaysOnTop(settings->AlwaysOnTop());
    _muteWnd.HideAnimation(settings->HideAnim(), settings->HideSpeed());
    _muteWnd.VisibleDuration(settings->HideDelay());

    UpdateIcon();
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);

    /* TODO: check whether we should show the OSD on startup or not. If so, post
     * a MSG_VOL_CHNG so that the volume level (or mute) is displayed: */
    SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
}

VolumeOSD::~VolumeOSD() {
    DestroyMenu(_deviceMenu);
    DestroyMenu(_menu);
    delete _icon;
    delete _volumeSlider;
    delete _callbackMeter;
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

    std::list<VolumeController::DeviceInfo> devices
        = _volumeCtrl->ListDevices();
    std::wstring currentDeviceId = _volumeCtrl->DeviceId();

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
    Settings *settings = Settings::Instance();
    Skin *skin = SkinManager::Instance()->CurrentSkin();

    /* Volume OSD */
    /* TODO: should make sure this isn't NULL! */
    _mWnd.BackgroundImage(skin->volumeBackground);

    if (skin->volumeMask != NULL) {
        _mWnd.EnableGlass(skin->volumeMask);
    }

    for (Meter *m : skin->volumeMeters) {
        _mWnd.AddMeter(m);
    }

    /* Add a callback meter with the default volume increment for sounds */
    _callbackMeter = new CallbackMeter(
        skin->DefaultVolumeUnits(), *this);
    _mWnd.AddMeter(_callbackMeter);

    /* Default volume increment */
    _defaultIncrement = (float) (10000 / skin->DefaultVolumeUnits()) / 10000.0f;
    CLOG(L"Default volume increment: %f", _defaultIncrement);

    _mWnd.Update();

    /* Mute OSD */
    /* TODO: NULL check*/
    _muteWnd.BackgroundImage(skin->muteBackground);

    if (skin->muteMask != NULL) {
        _muteWnd.EnableGlass(skin->muteMask);
    }
    _muteWnd.Update();

    /* Create clones for additional monitors */
    std::vector<Monitor> monitors = ActiveMonitors();
    for (unsigned int i = 1; i < monitors.size(); ++i) {
        _mWnd.Clone();
        _muteWnd.Clone();
    }
    UpdateWindowPositions(monitors);

    /* Set up notification icon */
    if (settings->NotifyIconEnabled()) {
        _iconImages = skin->volumeIconset;
        if (_iconImages.size() > 0) {
            _icon = new NotifyIcon(_hWnd, L"3RVX", _iconImages[0]);
        }
    }

    /* Enable sound effects, if any */
    if (settings->SoundEffectsEnabled()) {
        if (skin->volumeSound) {
            _soundPlayer = skin->volumeSound;
        }
    }
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
}

void VolumeOSD::MeterChangeCallback(int units) {
    if (_soundPlayer) {
        _soundPlayer->Play();
    }
}

void VolumeOSD::Hide() {
    _mWnd.Hide(false);
    _muteWnd.Hide(false);
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
        int vUnits = _iconImages.size() - 1;
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
        _icon->UpdateToolTip(_selectedDesc + L": Muted");
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
        }
        _volumeCtrl->Volume(
            (float) (currentUnit + unitIncrement) * _defaultIncrement);
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::Mute:
        _volumeCtrl->ToggleMute();
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::VolumeSlider:
        if (_volumeSlider->Visible()) {
            /* If the slider is already visible, user must want to close it. */
            _volumeSlider->Hide();
        } else {
            SendMessage(_hWnd, MSG_NOTIFYICON, NULL, WM_LBUTTONUP);
        }
        break;
    }
}

void VolumeOSD::ProcessVolumeHotkeys(HotkeyInfo &hki) {
    float currentVol = _volumeCtrl->Volume();
    HotkeyInfo::VolumeKeyArgTypes type = HotkeyInfo::VolumeArgType(hki);

    if (type == HotkeyInfo::VolumeKeyArgTypes::Percentage) {
        /* Deal with percentage-based amounts */
        float amount = (hki.ArgToDouble(0) / 100.0f);
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
    SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
}

void VolumeOSD::UpdateWindowPositions(std::vector<Monitor> &monitors) {
    PositionWindow(monitors[0], _mWnd);
    PositionWindow(monitors[0], _muteWnd);

    std::vector<LayeredWnd *> meterClones = _mWnd.Clones();
    std::vector<LayeredWnd *> muteClones = _muteWnd.Clones();
    for (unsigned int i = 1; i < monitors.size(); ++i) {
        PositionWindow(monitors[i], *meterClones[i - 1]);
        PositionWindow(monitors[i], *muteClones[i - 1]);
    }
}

void VolumeOSD::UpdateVolumeState() {
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);
    UpdateIcon();
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == MSG_VOL_CHNG) {
        float v = _volumeCtrl->Volume();
        bool muteState = _volumeCtrl->Muted();

        _volumeSlider->MeterLevels(v);
        UpdateIcon();

        if (wParam > 0) {
            /* We manually post a MSG_VOL_CHNG when modifying the volume with
             * hotkeys, so this CoreAudio-generated event can be ignored
             * by the OSD. */
            CLOG(L"Ignoring volume change notification generated by 3RVX");
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        CLOG(L"Volume change notification:\nNew level: %f\nPrevious: %f",
            v, _lastVolume);
        if (lParam == 0 && (muteState == _muted)) {
            if (abs(v - _lastVolume) < 0.0001f) {
                CLOG(L"No change in volume detected; ignoring event.");
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        _lastVolume = v;
        _muted = muteState;

        if (_volumeSlider->Visible() == false) {
            if (_volumeCtrl->Muted() || v == 0.0f) {
                _muteWnd.Show();
                _mWnd.Hide(false);
            } else {
                MeterLevels(v);
                _mWnd.Show();
                _muteWnd.Hide(false);
            }
            HideOthers(Volume);
        }

    } else if (message == MSG_VOL_DEVCHNG) {
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

    } else if (message == MSG_NOTIFYICON) {
        if (lParam == WM_LBUTTONUP) {
            _volumeSlider->MeterLevels(_volumeCtrl->Volume());
            _volumeSlider->Show();
        } else if (lParam == WM_RBUTTONUP) {
            POINT p;
            GetCursorPos(&p);
            SetForegroundWindow(hWnd);
            TrackPopupMenuEx(_menu, _menuFlags, p.x, p.y, _hWnd, NULL);
            PostMessage(hWnd, WM_NULL, 0, 0);
        }

    } else if (message == WM_COMMAND) {
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

    return DefWindowProc(hWnd, message, wParam, lParam);
}