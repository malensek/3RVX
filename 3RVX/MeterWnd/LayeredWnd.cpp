// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "LayeredWnd.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <VersionHelpers.h>

#include "../D3DDevice.h"
#include "../DisplayManager.h"
#include "../Error.h"
#include "../Logger.h"

LayeredWnd::LayeredWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance,
    Gdiplus::Bitmap *bitmap, DWORD exStyles) :
Window(className, title, hInstance,
    CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL,
    WS_POPUP, WS_EX_LAYERED | exStyles),
_transparency(255),
_visible(false) {

    Bitmap(bitmap);
}

LayeredWnd::~LayeredWnd() {
    delete _d3dDevice;
}

void LayeredWnd::UpdateWindow(RECT *dirtyRect) {
    BLENDFUNCTION bFunc;
    bFunc.AlphaFormat = AC_SRC_ALPHA;
    bFunc.BlendFlags = 0;
    bFunc.BlendOp = AC_SRC_OVER;
    bFunc.SourceConstantAlpha = _transparency;

    HDC screenDc = GetDC(GetDesktopWindow());
    HDC sourceDc = CreateCompatibleDC(screenDc);

    HBITMAP hBmp;
    _bitmap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBmp);
    HGDIOBJ hReplaced = SelectObject(sourceDc, hBmp);

    POINT pt = { 0, 0 };
    SIZE size = {
        static_cast<LONG>(_bitmap->GetWidth()),
        static_cast<LONG>(_bitmap->GetHeight())
    };

    UPDATELAYEREDWINDOWINFO lwInfo;
    lwInfo.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
    lwInfo.crKey = 0;
    lwInfo.dwFlags = ULW_ALPHA;
    lwInfo.hdcDst = screenDc;
    lwInfo.hdcSrc = sourceDc;
    lwInfo.pblend = &bFunc;
    lwInfo.pptDst = &_location;
    lwInfo.pptSrc = &pt;
    lwInfo.prcDirty = dirtyRect;
    lwInfo.psize = &size;

    UpdateLayeredWindowIndirect(Window::Handle(), &lwInfo);

    SelectObject(sourceDc, hReplaced);
    DeleteDC(sourceDc);
    DeleteObject(hBmp);
    ReleaseDC(GetDesktopWindow(), screenDc);
}

void LayeredWnd::UpdateTransparency() {
    BLENDFUNCTION bFunc;
    bFunc.AlphaFormat = AC_SRC_ALPHA;
    bFunc.BlendFlags = 0;
    bFunc.BlendOp = AC_SRC_OVER;
    bFunc.SourceConstantAlpha = _transparency;

    UPDATELAYEREDWINDOWINFO lwInfo;
    lwInfo.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
    lwInfo.crKey = 0;
    lwInfo.dwFlags = ULW_ALPHA;
    lwInfo.hdcDst = NULL;
    lwInfo.hdcSrc = NULL;
    lwInfo.pblend = &bFunc;
    lwInfo.pptDst = NULL;
    lwInfo.pptSrc = NULL;
    lwInfo.prcDirty = NULL;
    lwInfo.psize = NULL;

    UpdateLayeredWindowIndirect(Window::Handle(), &lwInfo);
}

bool LayeredWnd::EnableGlass(Gdiplus::Bitmap *mask) {
    if (mask == NULL) {
        return false;
    }

    /* Disable for Windows 8+ */
    if (IsWindows8OrGreater()) {
        return false;
    }

    /* Disable for Windows XP */
    if (IsWindowsXPOrGreater() == true && IsWindowsVistaOrGreater() == false) {
        return false;
    }

    _glassMask = mask;

    using namespace Gdiplus;
    ARGB searchArgb = 0xFF000000;

    unsigned int height = _glassMask->GetHeight();
    unsigned int width = _glassMask->GetWidth();

    Region glassRegion;
    glassRegion.MakeEmpty();
    bool match = false;

    /* One row of pixels is scanned at a time, so the height is 1. */
    Rect rec(0, 0, 0, 1);

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            Color pixelColor;
            _glassMask->GetPixel(x, y, &pixelColor);
            ARGB pixelArgb = pixelColor.GetValue();

            if (searchArgb == pixelArgb && (x + 1 != width)) {
                if (match) {
                    continue;
                }

                match = true;
                rec.X = x;
                rec.Y = y;
            } else if (match) {
                /* Reached the end of a matching line */
                match = false;
                rec.Width = x - rec.X;
                glassRegion.Union(rec);
            }
        }
    }

    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    blurBehind.fEnable = TRUE;

    Graphics g(_glassMask);
    blurBehind.hRgnBlur = glassRegion.GetHRGN(&g);

    HRESULT hr = DwmEnableBlurBehindWindow(Window::Handle(), &blurBehind);
    return SUCCEEDED(hr);
}

bool LayeredWnd::DisableGlass() {
    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE;
    blurBehind.fEnable = FALSE;
    HRESULT hr = DwmEnableBlurBehindWindow(Window::Handle(), &blurBehind);
    return SUCCEEDED(hr);
}

bool LayeredWnd::NoShowFullscreen() {
    return _noShowFull;
}

void LayeredWnd::NoShowFullscreen(bool enable) {
    _noShowFull = enable;
}

bool LayeredWnd::NoShowD3DOccluded() {
    return _noShowD3D;
}

void LayeredWnd::NoShowD3DOccluded(bool enable) {
    if (enable == true) {
        if (_d3dDevice == nullptr) {
            _d3dDevice = new D3DDevice(Window::Handle());
        }
    } else {
        if (_d3dDevice != nullptr) {
            delete _d3dDevice;
        }
    }
    _noShowD3D = enable;
}

void LayeredWnd::UpdateWindowPosition() {
    MoveWindow(Window::Handle(),
        _location.x, _location.y, _size.cx, _size.cy, FALSE);
}

void LayeredWnd::Show() {
    if (_visible == true) {
        return;
    }

    if (_noShowFull == true && DisplayManager::IsFullscreen(Window::Handle())) {
        CLOG(L"Not showing; fullscreen detected");
        return;
    }

    if (_noShowD3D == true && _d3dDevice->Occluded() == true) {
        CLOG(L"Not showing; D3D occlusion detected");
        return;
    }

    UpdateWindow();
    ShowWindow(Window::Handle(), SW_SHOW);
    _visible = true;
}

void LayeredWnd::Hide() {
    if (_visible == false) {
        return;
    }

    ShowWindow(Window::Handle(), SW_HIDE);
    _visible = false;
}

Gdiplus::Bitmap *LayeredWnd::Bitmap() {
    return _bitmap;
}

void LayeredWnd::Bitmap(Gdiplus::Bitmap *bitmap) {
    if (bitmap == NULL) {
        return;
    }

    _bitmap = bitmap;
    _size.cx = bitmap->GetWidth();
    _size.cy = bitmap->GetHeight();
    UpdateWindow();
}

bool LayeredWnd::AlwaysOnTop() {
    return (GetWindowLong(Window::Handle(), GWL_EXSTYLE) & WS_EX_TOPMOST) != 0;
}

void LayeredWnd::AlwaysOnTop(bool onTop) {
    if (onTop) {
        SetWindowPos(Window::Handle(), HWND_TOPMOST,
            X(), Y(), Width(), Height(), NULL);
    } else {
        SetWindowPos(Window::Handle(), HWND_NOTOPMOST,
            X(), Y(), Width(), Height(), NULL);
    }
}

byte LayeredWnd::Transparency() {
    return _transparency;
}

void LayeredWnd::Transparency(byte transparency) {
    _transparency = transparency;
    UpdateTransparency();
}

int LayeredWnd::X() {
    return _location.x;
}

void LayeredWnd::X(int x) {
    _location.x = x;
}

int LayeredWnd::Y() {
    return _location.y;
}

void LayeredWnd::Y(int y) {
    _location.y = y;
}

int LayeredWnd::Width() {
    return _size.cx;
}

int LayeredWnd::Height() {
    return _size.cy;
}

POINT LayeredWnd::Position() {
    return _location;
}

void LayeredWnd::Position(int x, int y) {
    _location.x = x;
    _location.y = y;
    UpdateWindowPosition();
}

LRESULT LayeredWnd::WndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    if (_glassMask && message == WM_DWMCOMPOSITIONCHANGED) {
        BOOL compositionEnabled = FALSE;
        HRESULT hr = DwmIsCompositionEnabled(&compositionEnabled);

        if (SUCCEEDED(hr)) {
            if (compositionEnabled) {
                EnableGlass(_glassMask);
            } else {
                DisableGlass();
            }
        }
    }

    return Window::WndProc(hWnd, message, wParam, lParam);
}