#pragma once

#include "MeterWnd\Animations\FadeOut.h"
#include "MeterWnd\MeterWnd.h"
#include "NotifyIcon.h"

class Skin;

class VolumeOSD {
public:
    VolumeOSD(HINSTANCE hInstance);
    void LoadSkin(Skin *skin);
    void MeterLevels();
    void MeterLevels(float value);

    void HideIcon();

private:
    HWND _hWnd;
    MeterWnd _mWnd;
    HWND _masterWnd;
    FadeOut _fout;
    NotifyIcon *_icon;
    HMENU _menu;
    UINT _menuFlags;
    std::wstring _settingsExe;

    void Hide();
    void AnimateIn();
    void AnimateOut();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};