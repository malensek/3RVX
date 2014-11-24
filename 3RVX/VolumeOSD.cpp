#include "VolumeOSD.h"

#include <Shlwapi.h>
#include <string>

#include "Monitor.h"
#include "Settings.h"
#include "Skin.h"

#define TIMER_HIDE 100
#define TIMER_ANIMIN 101
#define TIMER_ANIMOUT 102

#define MENU_SETTINGS 0
#define MENU_MIXER 1
#define MENU_EXIT 2
#define MENU_DEVICE 0xF000

VolumeOSD::VolumeOSD(HINSTANCE hInstance, Settings &settings) :
_mWnd(hInstance, L"3RVX-MasterVolumeOSD", L"3RVX-MasterVolumeOSD"),
_settings(settings),
_fout(_mWnd) {

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = NULL;
    wcex.lpfnWndProc = &VolumeOSD::StaticWndProc;
    wcex.cbClsExtra = NULL;
    wcex.cbWndExtra = NULL;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"3RVX-VolumeDispatcher";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wcex);
    /* throw exception if failed? */

    _hWnd = CreateWindowEx(
        NULL, L"3RVX-VolumeDispatcher", L"3RVX-VolumeDispatcher",
        NULL, NULL, NULL, NULL, NULL, HWND_MESSAGE, NULL, hInstance, this);

    _masterWnd = FindWindow(L"3RVXv3", L"3RVXv3");

    std::wstring skinXML = settings.SkinXML();
    LoadSkin(skinXML);

    /* Start the volume controller */
    _volumeCtrl = new CoreAudio(_hWnd);
    std::wstring device = settings.GetText("audioDevice");
    _volumeCtrl->Init(device);

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

    /* TODO: if set, we should update the volume level here to show the OSD
     * on startup. */
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

    /* Set up notification icon */
    Gdiplus::Bitmap *icon = Gdiplus::Bitmap::FromFile(L"Skins/Ignition/OSD/Mute.png");
    _icon = new NotifyIcon(_hWnd, L"3RVX", icon);
    _icon->UpdateToolTip(L"testing");
}

void VolumeOSD::MeterLevels(float level) {
    KillTimer(_hWnd, TIMER_ANIMOUT);
    _mWnd.MeterLevels(level);
    _mWnd.Update();
    _mWnd.Transparency(255);
    _mWnd.Show();
    SetTimer(_hWnd, TIMER_HIDE, 800, NULL);
}

void VolumeOSD::Hide() {
    _fout.Reset();
    SetTimer(_hWnd, TIMER_ANIMOUT, 15, NULL);
}

void VolumeOSD::AnimateOut() {
    bool animOver = _fout.Animate();
    if (animOver) {
        CLOG(L"Finished hiding window.");
        KillTimer(_hWnd, TIMER_ANIMOUT);
        _mWnd.Hide();
    }
}

void VolumeOSD::HideIcon() {
    delete _icon;
}

LRESULT CALLBACK
VolumeOSD::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    VolumeOSD* vOsd;

    if (message == WM_CREATE) {
        vOsd = (VolumeOSD*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) vOsd);
    } else {
        vOsd = (VolumeOSD*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!vOsd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return vOsd->WndProc(hWnd, message, wParam, lParam);
}

LRESULT
VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == MSG_VOL_CHNG) {
        float v = _volumeCtrl->Volume();
        QCLOG(L"Volume level: %.0f", v * 100.0f);
        MeterLevels(v);
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
    } else if (message == WM_TIMER) {
        switch (wParam) {
        case TIMER_HIDE:
            CLOG(L"Display duration has elapsed. Hiding window.");
            Hide();
            KillTimer(_hWnd, TIMER_HIDE);
            break;

        case TIMER_ANIMIN:
            break;

        case TIMER_ANIMOUT:
            AnimateOut();
            break;
        }
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