#include "VolumeOSD.h"

#include <Shlwapi.h>
#include <string>

#include "..\HotkeyInfo.h"
#include "..\MeterWnd\Meters\CallbackMeter.h"
#include "..\Monitor.h"
#include "..\Skin.h"
#include "..\Slider\VolumeSlider.h"

#define MENU_SETTINGS 0
#define MENU_MIXER 1
#define MENU_EXIT 2
#define MENU_DEVICE 0xF000

VolumeOSD::VolumeOSD() :
OSD(L"3RVX-VolumeDispatcher"),
_mWnd(L"3RVX-MasterVolumeOSD", L"3RVX-MasterVolumeOSD"),
_muteWnd(L"3RVX-MasterMuteOSD", L"3RVX-MasterMuteOSD")
{
    LoadSkin();
    Settings *settings = Settings::Instance();

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(_hWnd);
    std::wstring device = settings->AudioDeviceID();
    _volumeCtrl->Init(device);
    _selectedDesc = _volumeCtrl->DeviceDesc();

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

    FadeOut *fOut = new FadeOut();
    _mWnd.HideAnimation(fOut);
    _mWnd.VisibleDuration(settings->HideDelay());
    _muteWnd.HideAnimation(fOut);
    _muteWnd.VisibleDuration(settings->HideDelay());

    UpdateIcon();
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);

    /* TODO: check whether we should show the OSD on startup or not. If so, post
     * a MSG_VOL_CHNG so that the volume level (or mute) is displayed: */
    SendMessage(_hWnd, MSG_VOL_CHNG, NULL, NULL);
}

VolumeOSD::~VolumeOSD() {
    DestroyMenu(_deviceMenu);
    DestroyMenu(_menu);
    delete _icon;
    delete _volumeSlider;
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
    Skin *skin = settings->CurrentSkin();

    /* Volume OSD */
    /* TODO: should make sure this isn't NULL! */
    _mWnd.BackgroundImage(skin->volumeBackground);

    if (skin->volumeMask != NULL) {
        _mWnd.GlassMask(skin->volumeMask);
    }

    for (Meter *m : skin->volumeMeters) {
        _mWnd.AddMeter(m);
    }

    /* Add a callback meter with the default volume increment for sounds */
    CallbackMeter *callbackMeter = new CallbackMeter(
        skin->DefaultVolumeUnits(), *this);
    _mWnd.AddMeter(callbackMeter);

    /* Default volume increment */
    _defaultIncrement = (float) (10000 / skin->DefaultVolumeUnits()) / 10000.0f;
    CLOG(L"Default volume increment: %f", _defaultIncrement);

    _mWnd.Update();

    /* Mute OSD */
    /* TODO: NULL check*/
    _muteWnd.BackgroundImage(skin->muteBackground);

    if (skin->muteMask != NULL) {
        _muteWnd.GlassMask(skin->muteMask);
    }
    _muteWnd.Update();

    UpdateWindowPositions(MonitorHandles());

    /* Set up notification icon */
    if (settings->NotifyIconEnabled()) {
        _iconImages = skin->Iconset("volume");
        if (_iconImages.size() > 0) {
            _icon = new NotifyIcon(_hWnd, L"3RVX", _iconImages[0]);
        }
    }

    /* Enable sound effects, if any */
    if (settings->SoundEffectsEnabled()) {
        //_soundPlayer = new SoundPlayer();
    }
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
}

void VolumeOSD::MeterChangeCallback(int units) {
    CLOG(L"got callback");
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
    float currentVol = _volumeCtrl->Volume();
    switch (hki.action) {
    case HotkeyInfo::IncreaseVolume:
        UnMute();
        _volumeCtrl->Volume(currentVol + _defaultIncrement + 0.0001f);
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::DecreaseVolume:
        UnMute();
        _volumeCtrl->Volume(currentVol - _defaultIncrement - 0.0001f);
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
        break;

    case HotkeyInfo::Mute:
        _volumeCtrl->ToggleMute();
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, (LPARAM) 1);
        break;
    }
}

void VolumeOSD::UpdateWindowPositions(std::vector<HMONITOR> monitors) {
    PositionWindow(monitors[0], _mWnd);
    PositionWindow(monitors[0], _muteWnd);
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == MSG_VOL_CHNG) {
        float v = _volumeCtrl->Volume();
        _volumeSlider->MeterLevels(v);

        if (wParam > 0) {
            /* We manually post a MSG_VOL_CHNG when modifying the volume with
             * hotkeys, so this CoreAudio-generated event can be ignored
             * by the OSD. */
            CLOG(L"Ignoring volume change notification generated by 3RVX");
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        CLOG(L"Volume change notification:\nNew level: %f\nPrevious: %f",
            v, _previousVolume);
        if (lParam == 0) {
            if (abs(v - _previousVolume) < 0.0001f) {
                CLOG(L"No change in volume detected; ignoring event.");
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        _previousVolume = v;

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

        UpdateIcon();
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
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}