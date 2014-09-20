#include "LayeredWnd.h"
#include <iostream>

bool LayeredWnd::Init() {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = &LayeredWnd::StaticWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_className;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        return false;
    }

    m_hWnd = CreateWindowEx(
        WS_EX_TOOLWINDOW | WS_EX_LAYERED | WS_EX_NOACTIVATE,
        m_className, m_title,
        WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
        1, 1,
        NULL, NULL, m_hInstance, this);

    if (m_hWnd == NULL) {
        return false;
    }

    m_transparency = 255;

    m_location.x = 0;
    m_location.y = 0;

    m_size.cx = 1;
    m_size.cy = 1;

    return true;
}

void LayeredWnd::UpdateLayeredWnd() {
    BLENDFUNCTION bFunc;
    bFunc.AlphaFormat = AC_SRC_ALPHA;
    bFunc.BlendFlags = 0;
    bFunc.BlendOp = AC_SRC_OVER;
    bFunc.SourceConstantAlpha = m_transparency;

    HDC screenDc = GetDC(GetDesktopWindow());
    HDC sourceDc = CreateCompatibleDC(screenDc);

    HBITMAP hBmp;
    m_buffer->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBmp);
    HGDIOBJ hReplaced = SelectObject(sourceDc, hBmp);

    POINT pt = { 0, 0 };
    SIZE size = { m_buffer->GetWidth(), m_buffer->GetHeight() };

    UPDATELAYEREDWINDOWINFO lwInfo;
    lwInfo.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
    lwInfo.crKey = 0;
    lwInfo.dwFlags = ULW_ALPHA;
    lwInfo.hdcDst = screenDc;
    lwInfo.hdcSrc = sourceDc;
    lwInfo.pblend = &bFunc;
    lwInfo.pptDst = &m_location;
    lwInfo.pptSrc = &pt;
    lwInfo.prcDirty = m_dirtyRect;
    lwInfo.psize = &size;

    UpdateLayeredWindowIndirect(m_hWnd, &lwInfo);

    SelectObject(sourceDc, hReplaced);
    DeleteDC(sourceDc);
    DeleteObject(hBmp);
    ReleaseDC(GetDesktopWindow(), screenDc);
}

void LayeredWnd::Show() {
    UpdateLocation();
    ShowWindow(m_hWnd, SW_SHOW);
}

void LayeredWnd::Hide() {
    ShowWindow(m_hWnd, SW_HIDE);
}

void LayeredWnd::UpdateTransparency() {
    BLENDFUNCTION bFunc;
    bFunc.AlphaFormat = AC_SRC_ALPHA;
    bFunc.BlendFlags = 0;
    bFunc.BlendOp = AC_SRC_OVER;
    bFunc.SourceConstantAlpha = m_transparency;

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

    UpdateLayeredWindowIndirect(m_hWnd, &lwInfo);
}

LayeredWnd::~LayeredWnd() {
    return;
}

//
// UpdateLocation()
//
// Uses MoveWindow() to set the window's X,Y coordinates its size.
//
void LayeredWnd::UpdateLocation() {
    MoveWindow(m_hWnd, m_location.x, m_location.y, m_size.cx, m_size.cy, FALSE);
}


LRESULT CALLBACK
LayeredWnd::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    LayeredWnd* lWnd;

    if (message == WM_CREATE) {
        lWnd = (LayeredWnd*)((LPCREATESTRUCT)lParam)->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lWnd);
    } else {
        lWnd = (LayeredWnd*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if (!lWnd) {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }

    return lWnd->WndProc(message, wParam, lParam);
}

LRESULT LayeredWnd::WndProc(UINT message, WPARAM wParam, LPARAM lParam) {

    return DefWindowProc(m_hWnd, message, wParam, lParam);
}


// - GETTERS --------------------------------------------------------------- //

int LayeredWnd::X() const {
    return m_location.x;
}

void LayeredWnd::X(int x) {
    m_location.x = x;
}

int LayeredWnd::Y() const {
    return m_location.y;
}

void LayeredWnd::Y(int y) {
    m_location.y = y;
}

int LayeredWnd::Height() const {
    return m_size.cy;
}

int LayeredWnd::Width() const {
    return m_size.cx;
}

byte LayeredWnd::Transparency() const {
    return m_transparency;
}

void LayeredWnd::Transparency(byte transparency) {
    m_transparency = transparency;
    UpdateTransparency();
}

HWND LayeredWnd::Hwnd() const {
    return m_hWnd;
}

Gdiplus::Bitmap *LayeredWnd::Image() {
    return m_buffer;
}

void LayeredWnd::Image(Gdiplus::Bitmap *image) {
    m_buffer = image;
    m_size.cx = image->GetWidth();
    m_size.cy = image->GetHeight();
    UpdateLayeredWnd();
}