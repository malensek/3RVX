#pragma once

#include <Windows.h>
#include <list>

#define MSG_NOTIFYICON WM_APP + 200

class NotifyIcon {
public:
    NotifyIcon(HWND hWnd, std::wstring caption, std::list<std::wstring> icons);
    ~NotifyIcon();

    NOTIFYICONDATA IconData();
    NOTIFYICONIDENTIFIER IconID();

private:
    static int ids;
    int _id;
    int _level;
    std::wstring _caption;
    NOTIFYICONDATA _nid;
    NOTIFYICONIDENTIFIER _nii;
};