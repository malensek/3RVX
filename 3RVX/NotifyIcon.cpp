#include "NotifyIcon.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "Logger.h"

int NotifyIcon::ids = 0;

NotifyIcon::NotifyIcon(HWND hWnd, std::wstring tip, Gdiplus::Bitmap *icon) :
_icon(icon),
_tip(tip) {

    using Gdiplus::Bitmap;

    _id = NotifyIcon::ids++;

    _nid = {};
    _nid.cbSize = { sizeof(_nid) };
    _nid.hWnd = hWnd;
    _nid.uID = _id;
    _nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    _nid.uCallbackMessage = MSG_NOTIFYICON;

    HICON hIcon;
    _icon->GetHICON(&hIcon);
    _nid.hIcon = hIcon;

    wcscpy_s(_nid.szTip, 128, tip.c_str());

    Shell_NotifyIcon(NIM_ADD, &_nid);

    _nii = {};
    _nii.cbSize = { sizeof(_nii) };
    _nii.hWnd = hWnd;
    _nii.uID = _id;
}

void NotifyIcon::UpdateToolTip(std::wstring newTip) {
    wcscpy_s(_nid.szTip, 128, newTip.c_str());
    Shell_NotifyIcon(NIM_MODIFY, &_nid);
}

NotifyIcon::~NotifyIcon() {
    Shell_NotifyIcon(NIM_DELETE, &_nid);
}

NOTIFYICONDATA NotifyIcon::IconData() {
    return _nid;
}

NOTIFYICONIDENTIFIER NotifyIcon::IconID() {
    return _nii;
}