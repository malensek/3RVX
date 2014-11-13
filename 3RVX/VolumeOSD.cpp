#include "VolumeOSD.h"

#include <Shlwapi.h>
#include <string>

#include "Monitor.h"
#include "Skin.h"

#define TIMER_HIDE 100
#define TIMER_ANIMIN 101
#define TIMER_ANIMOUT 102

#define MENU_SETTINGS 0
#define MENU_MIXER 1
#define MENU_EXIT 2

VolumeOSD::VolumeOSD(HINSTANCE hInstance) :
_mWnd(hInstance, L"3RVX-MasterVolumeOSD", L"3RVX-MasterVolumeOSD"),
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

    /* Load notification icons */
    std::list<std::wstring> l;
    _icon = new NotifyIcon(_hWnd, L"3RVX", l);

    /* Set up context menu */
    _menu = CreatePopupMenu();
    InsertMenu(_menu, -1, MF_ENABLED, MENU_SETTINGS, L"Settings");
    InsertMenu(_menu, -1, MF_ENABLED, MENU_MIXER, L"Mixer");
    InsertMenu(_menu, -1, MF_ENABLED, MENU_EXIT, L"Exit");

    _menuFlags = TPM_RIGHTBUTTON;
    if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
        _menuFlags |= TPM_RIGHTALIGN;
    } else {
        _menuFlags |= TPM_LEFTALIGN;
    }

    wchar_t path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    PathRemoveFileSpec(path);
    _appDir = std::wstring(path);
    _settingsExe = std::wstring(path) + L"\\SettingsUI.exe";
    CLOG(L"App location: %s", _appDir);
}

void VolumeOSD::LoadSkin(Skin *skin) {
    Gdiplus::Bitmap *bg = skin->OSDBgImg("volume");
    _mWnd.BackgroundImage(bg);

    std::list<Meter*> meters = skin->Meters("volume");
    for each (Meter *m in meters) {
        _mWnd.AddMeter(m);
    }

    _mWnd.Update();
    HMONITOR monitor = Monitor::Default();
    const int mWidth = Monitor::Width(monitor);
    const int mHeight = Monitor::Height(monitor);
    _mWnd.X(mWidth / 2 - _mWnd.Width() / 2);
    _mWnd.Y(mHeight - _mWnd.Height() - 140);
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
    if (message == WM_TIMER) {
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
        switch (LOWORD(wParam)) {
        case MENU_SETTINGS:
            CLOG(L"Opening Settings: %s", _settingsExe.c_str());
            ShellExecute(NULL, L"open", _settingsExe.c_str(), NULL, NULL, 0);
            break;

        case MENU_MIXER:
            CLOG(L"Menu: Mixer");

            break;

        case MENU_EXIT:
            CLOG(L"Menu: Exit: %d", _masterWnd);
            SendMessage(_masterWnd, WM_CLOSE, NULL, NULL);
            break;
        }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}