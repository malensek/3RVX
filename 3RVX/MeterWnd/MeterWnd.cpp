#include "MeterWnd.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "Animation.h"

#define TIMER_HIDE 100
#define TIMER_ANIMIN 101
#define TIMER_ANIMOUT 102

MeterWnd::MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance,
    Animation *hideAnimation, Animation *showAnimation, int visibleDuration) :
_hInstance(hInstance),
_className(className),
_title(title),
_hideAnimation(hideAnimation),
_showAnimation(showAnimation),
_visibleDuration(visibleDuration),
_visible(false) {
    if (_hInstance == NULL) {
        _hInstance = (HINSTANCE) GetModuleHandle(NULL);
    }

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = &MeterWnd::StaticWndProc;
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
        /* throw exception */
    }

    _location.x = 0;
    _location.y = 0;
    _size.cx = 1;
    _size.cy = 1;
    _transparency = 255;

    _hWnd = CreateWindowEx(
        WS_EX_TOOLWINDOW |
        WS_EX_LAYERED |
        WS_EX_NOACTIVATE |
        WS_EX_TOPMOST |
        WS_EX_TRANSPARENT,
        _className, _title,
        WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        _size.cx, _size.cy,
        NULL, NULL, _hInstance, this);

    if (_hWnd == NULL) {
        /* throw exception */
    }
}

MeterWnd::~MeterWnd() {
    DestroyWindow(_hWnd);
}

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

void MeterWnd::ApplyGlass(Gdiplus::Bitmap *glassMask) {
    using namespace Gdiplus;
    Color searchColor(0, 0, 0);
    ARGB searchArgb = 0xFF000000;

    unsigned int height = glassMask->GetHeight();
    unsigned int width = glassMask->GetWidth();

    Region reg;
    reg.MakeEmpty();
    bool match = false;
    Rect rec(0, 0, 0, 1); //scan one row of pixels at a time

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            Color pixelColor;
            glassMask->GetPixel(x, y, &pixelColor);
            ARGB pixelArgb = pixelColor.GetValue();

            if ((searchArgb & pixelArgb) == searchArgb && (x + 1 != width)) {
                if (match) {
                    continue;
                }

                match = true;
                rec.X = x;
                rec.Y = y;
            } else if (match) {
                match = false;
                rec.Width = x - rec.X;
                reg.Union(rec);
            }
        }
    }

    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    blurBehind.fEnable = true;

    Graphics g(glassMask);
    HRGN hReg = reg.GetHRGN(&g);
    blurBehind.hRgnBlur = reg.GetHRGN(&g);

    DwmEnableBlurBehindWindow(_hWnd, &blurBehind);
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

void MeterWnd::HideAnimation(Animation *anim) {
    _hideAnimation = anim;
}

void MeterWnd::VisibleDuration(int duration) {
    _visibleDuration = duration;
}

void MeterWnd::BackgroundImage(Gdiplus::Bitmap *background) {
    _background = background;
    _size.cx = background->GetWidth();
    _size.cy = background->GetHeight();
}

void MeterWnd::Show(bool animate) {
    //TODO: animation support
    if (_visible == false) {
        UpdateLocation();
        ShowWindow(_hWnd, SW_SHOW);
        _visible = true;
    }

    if (_visibleDuration > 0) {
        SetTimer(_hWnd, TIMER_HIDE, _visibleDuration, NULL);
        KillTimer(_hWnd, TIMER_ANIMOUT);

        if (_hideAnimation) {
            _hideAnimation->Reset(this);
        }
    }
}

void MeterWnd::Hide(bool animate) {
    if (_visible == false) {
        return;
    }

    if (animate && _hideAnimation) {
        SetTimer(_hWnd, TIMER_ANIMOUT, 15, NULL);
    } else {
        ShowWindow(_hWnd, SW_HIDE);
        _visible = false;
    }
}

void MeterWnd::AnimateOut() {
    bool animOver = _hideAnimation->Animate(this);
    if (animOver) {
        CLOG(L"Finished hide animation.");
        KillTimer(_hWnd, TIMER_ANIMOUT);
        ShowWindow(_hWnd, SW_HIDE);
        _visible = false;
    }
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

void MeterWnd::Move(int x, int y) {
    _location.x = x;
    _location.y = y;
    UpdateLocation();
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
    if (message == WM_TIMER) {
        switch (wParam) {
        case TIMER_HIDE:
            CLOG(L"Display duration has elapsed. Hiding window.");
            Hide();
            KillTimer(_hWnd, TIMER_HIDE);
            break;

        case TIMER_ANIMIN:
            break;

        case TIMER_ANIMOUT:
            AnimateOut();
            break;
        }
    }

    return DefWindowProc(_hWnd, message, wParam, lParam);
}

