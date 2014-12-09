#include "VolumeOSD.h"

#include <Shlwapi.h>
#include <string>

#include "..\Monitor.h"
#include "..\Skin.h"

#define MENU_SETTINGS 0
#define MENU_MIXER 1
#define MENU_EXIT 2
#define MENU_DEVICE 0xF000

VolumeOSD::VolumeOSD(HINSTANCE hInstance, Settings &settings) :
OSD(hInstance, L"3RVX-VolumeDispatcher", settings),
_mWnd(hInstance, L"3RVX-MasterVolumeOSD", L"3RVX-MasterVolumeOSD", NULL, NULL, 800),
_muteWnd(hInstance, L"3RVX-MasterMuteOSD", L"3RVX-MasterMuteOSD", NULL, NULL, 800)
{
    std::wstring skinXML = settings.SkinXML();
    LoadSkin(skinXML);

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(_hWnd);
    std::wstring device = settings.GetText("audioDevice");
    _volumeCtrl->Init(device);
    _selectedDesc = _volumeCtrl->DeviceDesc();

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
    _muteWnd.HideAnimation(fOut);

    /* TODO: if set, we should update the volume level here to show the OSD
     * on startup. */
    UpdateIconTip();
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

void VolumeOSD::LoadSkin(std::wstring skinXML) {
    Skin skin(skinXML);

    Gdiplus::Bitmap *bg = skin.OSDBgImg("volume");
    _mWnd.BackgroundImage(bg);

    std::list<Meter*> meters = skin.Meters("volume");
    for each (Meter *m in meters) {
        _mWnd.AddMeter(m);
    }

    _mWnd.Update();
    HMONITOR monitor = Monitor::Default();
    const int mWidth = Monitor::Width(monitor);
    const int mHeight = Monitor::Height(monitor);
    _mWnd.X(mWidth / 2 - _mWnd.Width() / 2);
    _mWnd.Y(mHeight - _mWnd.Height() - 140);

    _muteBg = skin.OSDBgImg("mute");
    _muteWnd.BackgroundImage(_muteBg);
    _muteWnd.Update();
    _muteWnd.X(mWidth / 2 - _muteWnd.Width() / 2);
    _muteWnd.Y(mHeight - _muteWnd.Height() - 140);


    /* Set up notification icon */
    skin.Iconset("volume");
    Gdiplus::Bitmap *iconBmp = Gdiplus::Bitmap::FromFile(L"Skins/Ignition/OSD/Mute.png");
    HICON icon;
    iconBmp->GetHICON(&icon);
    _icon = new NotifyIcon(_hWnd, L"3RVX", icon);
    _icon->UpdateToolTip(L"testing");
    DestroyIcon(icon);
    Gdiplus::Bitmap *iconBmp2 = Gdiplus::Bitmap::FromFile(L"Skins/Ignition/OSD/Eject.png");
    iconBmp2->GetHICON(&icon);
    _icon->UpdateIcon(icon);
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

void VolumeOSD::UpdateIconTip() {
    if (_volumeCtrl->Muted()) {
        _icon->UpdateToolTip(_selectedDesc + L": Muted");
    } else {
        float v = _volumeCtrl->Volume();
        std::wstring perc = std::to_wstring((int) (v * 100.0f));
        std::wstring level = _selectedDesc + L": " + perc + L"%";
        _icon->UpdateToolTip(level);
    }
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == MSG_VOL_CHNG) {
        HideOthers(Volume);
        float v = _volumeCtrl->Volume();
        if (_volumeCtrl->Muted() || v == 0.0f) {
            _mWnd.Hide(false);
            _muteWnd.Show();
        } else {
            _muteWnd.Hide(false);
            QCLOG(L"Volume level: %.0f", v * 100.0f);
            MeterLevels(v);
            _mWnd.Show();
        }
        UpdateIconTip();
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
    } else if (message == MSG_NOTIFYICON) {
        if (lParam == WM_LBUTTONUP || lParam == WM_RBUTTONUP) {
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
            CLOG(L"Opening Settings App: %s", Settings::SettingsApp().c_str());
            ShellExecute(NULL, L"open",
                Settings::SettingsApp().c_str(), NULL, NULL, 0);
            break;

        case MENU_MIXER:
            CLOG(L"Menu: Mixer");

            break;

        case MENU_EXIT:
            CLOG(L"Menu: Exit: %d", _masterWnd);
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