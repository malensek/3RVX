#pragma once

#include "MeterWnd\Meters\MeterTypes.h"
#include "MeterWnd\MeterWnd.h"
#include "MeterWnd\Meter.h"

class VolumeOSD {
public:
    VolumeOSD(HINSTANCE hInstance):
        _mWnd(hInstance, L"3RVX-MasterVolumeOSD", L"3RVX-MasterVolumeOSD") {

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
        CLOG(L"%d", _hWnd);
    };

    void LoadSkin(std::wstring skinName);
    void MeterLevels();
    void MeterLevels(float value);

private:
    HWND _hWnd;
    MeterWnd _mWnd;

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};