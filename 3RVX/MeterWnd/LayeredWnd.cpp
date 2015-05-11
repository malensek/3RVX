#include "LayeredWnd.h"

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <VersionHelpers.h>

#include "..\Error.h"

LayeredWnd::LayeredWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance,
    Gdiplus::Bitmap *bitmap, DWORD exStyles) :
_className(className),
_hInstance(hInstance),
_title(title),
_transparency(255),
_visible(false) {

    if (_hInstance == NULL) {
        _hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = &LayeredWnd::StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _className;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        throw SYSERR_REGISTERCLASS;
    }

    DWORD styles = WS_EX_LAYERED | exStyles;

    _hWnd = CreateWindowEx(
        styles,
        _className, _title,
        WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        _size.cx, _size.cy,
        NULL, NULL, _hInstance, this);

    if (_hWnd == NULL) {
        throw SYSERR_CREATEWINDOW;
    }

    Bitmap(bitmap);
}

LayeredWnd::~LayeredWnd() {
    DestroyWindow(_hWnd);
    UnregisterClass(_className, _hInstance);
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
    SIZE size = { _bitmap->GetWidth(), _bitmap->GetHeight() };

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

    UpdateLayeredWindowIndirect(_hWnd, &lwInfo);

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

    UpdateLayeredWindowIndirect(_hWnd, &lwInfo);
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

    HRESULT hr = DwmEnableBlurBehindWindow(_hWnd, &blurBehind);
    return SUCCEEDED(hr);
}

bool LayeredWnd::DisableGlass() {
    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE;
    blurBehind.fEnable = FALSE;
    HRESULT hr = DwmEnableBlurBehindWindow(_hWnd, &blurBehind);
    return SUCCEEDED(hr);
}

void LayeredWnd::UpdateWindowPosition() {
    MoveWindow(_hWnd, _location.x, _location.y, _size.cx, _size.cy, FALSE);
}

void LayeredWnd::Show() {
    if (_visible == true) {
        return;
    }

    UpdateWindow();
    ShowWindow(_hWnd, SW_SHOW);
    _visible = true;
}

void LayeredWnd::Hide() {
    if (_visible == false) {
        return;
    }

    ShowWindow(_hWnd, SW_HIDE);
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
    return GetWindowLong(_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST;
}

void LayeredWnd::AlwaysOnTop(bool onTop) {
    if (onTop) {
        SetWindowPos(_hWnd, HWND_TOPMOST, X(), Y(), Width(), Height(), NULL);
    } else {
        SetWindowPos(_hWnd, HWND_NOTOPMOST, X(), Y(), Width(), Height(), NULL);
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

LRESULT CALLBACK
LayeredWnd::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LayeredWnd* wnd;

    if (message == WM_CREATE) {
        wnd = (LayeredWnd *) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) wnd);
    } else {
        wnd = (LayeredWnd *) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!wnd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return wnd->WndProc(message, wParam, lParam);
}

LRESULT LayeredWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

    return DefWindowProc(_hWnd, message, wParam, lParam);
}