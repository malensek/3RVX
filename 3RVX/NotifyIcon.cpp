#include "NotifyIcon.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "Logger.h"

int NotifyIcon::ids = 0;

NotifyIcon::NotifyIcon(HWND hWnd, std::wstring caption,
    std::list<std::wstring> icons) :
    _level(-1),
    _caption(caption) {

    using Gdiplus::Bitmap;

    _id = NotifyIcon::ids++;

    _nid = {};
    _nid.cbSize = { sizeof(_nid) };
    _nid.hWnd = hWnd;
    _nid.uID = _id;
    _nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    _nid.uCallbackMessage = MSG_NOTIFYICON;
    HICON icon;
    Bitmap *iconBmp = new Bitmap(L"Skins/Ignition/Notification Icons/0.ico");
    iconBmp->GetHICON(&icon);
    _nid.hIcon = icon;

    wcscpy_s(_nid.szTip, 128, L"3RVX");

    Shell_NotifyIcon(NIM_ADD, &_nid);

//    _nid.uVersion = NOTIFYICON_VERSION_4;
//    Shell_NotifyIcon(NIM_SETVERSION, &_nid);

    _nii = {};
    _nii.cbSize = { sizeof(_nii) };
    _nii.hWnd = hWnd;
    _nii.uID = _id;
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