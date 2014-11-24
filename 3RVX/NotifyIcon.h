#pragma once

#include <Windows.h>
#include <string>

#define MSG_NOTIFYICON WM_APP + 200

class NotifyIcon {
public:
    NotifyIcon(HWND hWnd, std::wstring tip, HICON icon);
    ~NotifyIcon();

    NOTIFYICONDATA IconData();
    NOTIFYICONIDENTIFIER IconID();

    void UpdateIcon(HICON icon);
    void UpdateToolTip(std::wstring newTip);

private:
    static int ids;

    HICON _icon;
    int _id;
    std::wstring _tip;

    NOTIFYICONDATA _nid;
    NOTIFYICONIDENTIFIER _nii;
};