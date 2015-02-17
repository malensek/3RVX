#include "VolumeOSD.h"

#include <Shlwapi.h>
#include <string>

#include "..\HotkeyInfo.h"
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

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(_hWnd);
    std::wstring device = Settings::Instance()->AudioDeviceID();
    _volumeCtrl->Init(device);
    _selectedDesc = _volumeCtrl->DeviceDesc();

    /* Create the slider */
    _volumeSlider = new VolumeSlider(*_volumeCtrl);

    /* Set up context menu */
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

    FadeOut *fOut = new FadeOut();
    _mWnd.HideAnimation(fOut);
    _mWnd.VisibleDuration(800);
    _muteWnd.HideAnimation(fOut);
    _muteWnd.VisibleDuration(800);

    UpdateIcon();
    float v = _volumeCtrl->Volume();
    MeterLevels(v);
    _volumeSlider->MeterLevels(v);

    /* TODO: check whether we should show the OSD on startup or not. If so, post
     * a MSG_VOL_CHNG so that the volume level (or mute) is displayed: */
    SendMessage(_hWnd, MSG_VOL_CHNG, NULL, NULL);
}

VolumeOSD::~VolumeOSD() {

    /* DestroyIcon() */
}

void VolumeOSD::UpdateDeviceMenu() {
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
    Skin *skin = Settings::Instance()->CurrentSkin();

    Gdiplus::Bitmap *bg = skin->OSDBgImg("volume");
    _mWnd.BackgroundImage(bg);

    std::list<Meter*> meters = skin->VolumeMeters();
    for (Meter *m : meters) {
        _mWnd.AddMeter(m);
    }

    /* Default volume increment */
    _defaultIncrement = (float) (10000 / skin->DefaultVolumeUnits()) / 10000.0f;
    CLOG(L"Default volume increment: %f", _defaultIncrement);

    HMONITOR monitor = Monitor::Primary();

    _mWnd.Update();
    PositionWindow(monitor, _mWnd);

    _muteBg = skin->OSDBgImg("mute");
    _muteWnd.BackgroundImage(_muteBg);
    _muteWnd.Update();
    PositionWindow(monitor, _muteWnd);

    /* Set up notification icon */
    _iconImages = skin->Iconset("volume");
    if (_iconImages.size() > 0) {
        _icon = new NotifyIcon(_hWnd, L"3RVX", _iconImages[0]);
    }
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
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

void VolumeOSD::ProcessHotkeys(HotkeyInfo &hki) {
    float currentVol = _volumeCtrl->Volume();
    switch (hki.action) {
    case HotkeyInfo::IncreaseVolume:
        _volumeCtrl->Volume(currentVol + _defaultIncrement);
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, NULL);
        break;

    case HotkeyInfo::DecreaseVolume:
        _volumeCtrl->Volume(currentVol - _defaultIncrement);
        SendMessage(_hWnd, MSG_VOL_CHNG, NULL, NULL);
        break;

    case HotkeyInfo::Mute:
        _volumeCtrl->ToggleMute();
        break;
    }
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == MSG_VOL_CHNG) {
        float v = _volumeCtrl->Volume();

        _volumeSlider->MeterLevels(v);

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