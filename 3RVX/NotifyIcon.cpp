#include "NotifyIcon.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "Logger.h"

bool NotifyIcon::Init(HWND hWnd)
{
    using Gdiplus::Bitmap;

    NOTIFYICONDATA nIcon;
    //nIcon.cbSize = 
    //nIcon.dwInfoFlags
    //nIcon.dwState = 
    nIcon.cbSize = sizeof(NOTIFYICONDATA);
    nIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nIcon.uID = 1;
    nIcon.uVersion = NOTIFYICON_VERSION;
    HICON icon;
    Bitmap *iconBmp = new Bitmap(L"c:\\icon.ico");
    iconBmp->GetHICON(&icon);
    nIcon.hIcon = icon;

    nIcon.uCallbackMessage = WM_APP + 12;

    nIcon.hWnd = hWnd;

    Shell_NotifyIcon(NIM_ADD, &nIcon);

    NOTIFYICONIDENTIFIER nid;
    nid.cbSize = sizeof(NOTIFYICONIDENTIFIER);
    nid.hWnd = hWnd;
    nid.uID = 1;

    RECT *iconLoc = NULL;
    if (Shell_NotifyIconGetRect(&nid, iconLoc) == S_OK)
        CLOG(L"%d %d", iconLoc->left, iconLoc->top);

    return true;
}