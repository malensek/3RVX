#include "VolumeOSD.h"

#include <string>

#include "Monitor.h"
#include "Skin.h"

#define TIMER_HIDE 100
#define TIMER_ANIMIN 101
#define TIMER_ANIMOUT 102

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
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}