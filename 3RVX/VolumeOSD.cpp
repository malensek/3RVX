#include "VolumeOSD.h"

#include <string>

#include "Monitor.h"
#include "Skin.h"

void VolumeOSD::LoadSkin(std::wstring skinName) {
    Skin skin(skinName);

    Gdiplus::Bitmap *bg = skin.OSDBgImg("volume");
    _mWnd.SetBackgroundImage(bg);

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
}

void VolumeOSD::MeterLevels(float level) {
    _mWnd.MeterLevels(level);
    _mWnd.Update();
    _mWnd.Show();
    SetTimer(_hWnd, 100, 800, NULL);
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

LRESULT VolumeOSD::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_TIMER) {
        CLOG(L"------> timer");
        KillTimer(_hWnd, 100);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}