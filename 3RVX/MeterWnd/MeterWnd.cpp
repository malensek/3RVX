#include "MeterWnd.h"

void MeterWnd::Update()
{
    CLOG(L"Updating meter window");
    using namespace Gdiplus;

    bool dirty = (_composite == NULL);

    for (Meter *meter : _meters) {
        if (meter->Dirty() == true) {
            dirty = true;
            break;
        }
    }

    if (dirty) {
        CLOG(L"Contents have changed; redrawing");

        if (_composite) {
            delete _composite;
        }

        Rect bgRect(0, 0, _background->GetWidth(), _background->GetHeight());
        _composite = _background->Clone(bgRect, PixelFormat32bppARGB);
        Graphics graphics(_composite);

        for (Meter *meter : _meters) {
            CLOG(L"Drawing meter:\n%s", meter->ToString().c_str());
            meter->Draw(_composite, &graphics);
        }
    }

    UpdateLayeredWnd();
}

void MeterWnd::UpdateLayeredWnd() {
    BLENDFUNCTION bFunc;
    bFunc.AlphaFormat = AC_SRC_ALPHA;
    bFunc.BlendFlags = 0;
    bFunc.BlendOp = AC_SRC_OVER;
    bFunc.SourceConstantAlpha = _transparency;

    HDC screenDc = GetDC(GetDesktopWindow());
    HDC sourceDc = CreateCompatibleDC(screenDc);

    HBITMAP hBmp;
    _composite->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBmp);
    HGDIOBJ hReplaced = SelectObject(sourceDc, hBmp);

    POINT pt = { 0, 0 };
    SIZE size = { _composite->GetWidth(), _composite->GetHeight() };

    UPDATELAYEREDWINDOWINFO lwInfo;
    lwInfo.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
    lwInfo.crKey = 0;
    lwInfo.dwFlags = ULW_ALPHA;
    lwInfo.hdcDst = screenDc;
    lwInfo.hdcSrc = sourceDc;
    lwInfo.pblend = &bFunc;
    lwInfo.pptDst = &_location;
    lwInfo.pptSrc = &pt;
    lwInfo.prcDirty = _dirtyRect;
    lwInfo.psize = &size;

    UpdateLayeredWindowIndirect(_hWnd, &lwInfo);

    SelectObject(sourceDc, hReplaced);
    DeleteDC(sourceDc);
    DeleteObject(hBmp);
    ReleaseDC(GetDesktopWindow(), screenDc);
}

void MeterWnd::UpdateTransparency() {
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

//
// UpdateLocation()
//
// Uses MoveWindow() to set the window's X,Y coordinates its size.
//
void MeterWnd::UpdateLocation() {
    MoveWindow(_hWnd, _location.x, _location.y, Width(), Height(), FALSE);
}

void MeterWnd::AddMeter(Meter *meter)
{
    _meters.push_back(meter);
}

void MeterWnd::MeterLevels(float value)
{
    for (Meter *meter : _meters) {
        meter->Value(value);
    }
}

void MeterWnd::BackgroundImage(Gdiplus::Bitmap *background) {
    _background = background;
    _size.cx = background->GetWidth();
    _size.cy = background->GetHeight();
}

void MeterWnd::Show() {
    UpdateLocation();
    ShowWindow(_hWnd, SW_SHOW);
}

void MeterWnd::Hide() {
    ShowWindow(_hWnd, SW_HIDE);
}

int MeterWnd::X() const {
    return _location.x;
}

void MeterWnd::X(int x) {
    _location.x = x;
}

int MeterWnd::Y() const {
    return _location.y;
}

void MeterWnd::Y(int y) {
    _location.y = y;
}

int MeterWnd::Width() const {
    return _size.cx;
}

int MeterWnd::Height() const {
    return _size.cy;
}

byte MeterWnd::Transparency() const {
    return _transparency;
}

void MeterWnd::Transparency(byte transparency) {
    _transparency = transparency;
    UpdateTransparency();
}

LRESULT CALLBACK
MeterWnd::StaticWndProc(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    MeterWnd* wnd;

    if (message == WM_CREATE) {
        wnd = (MeterWnd*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) wnd);
    } else {
        wnd = (MeterWnd*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!wnd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return wnd->WndProc(message, wParam, lParam);
}

LRESULT MeterWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {
    /* Handle messages here */
    return DefWindowProc(_hWnd, message, wParam, lParam);
}

