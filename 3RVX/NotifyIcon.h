#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <string>

#define MSG_NOTIFYICON WM_APP + 200

class NotifyIcon {
public:
    NotifyIcon(HWND hWnd, std::wstring tip, Gdiplus::Bitmap *icon);
    ~NotifyIcon();

    NOTIFYICONDATA IconData();
    NOTIFYICONIDENTIFIER IconID();

    void UpdateToolTip(std::wstring newTip);

private:
    static int ids;

    Gdiplus::Bitmap *_icon;
    int _id;
    std::wstring _tip;
    NOTIFYICONDATA _nid;
    NOTIFYICONIDENTIFIER _nii;
};