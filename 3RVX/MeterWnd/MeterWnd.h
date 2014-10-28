#pragma once

#include <list>
#include "Meter.h"

#define TIMER_SHOWANIM    0x01
#define TIMER_HIDEANIM    0x02
#define TIMER_DELAY       0x03

class MeterWnd {
public:
    MeterWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title):
    _hInstance(hInstance),
    _className(className),
    _title(title) {
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
    };

	void Update();

    virtual void Show();
    virtual void Hide();

    int X() const;
    int Y() const;
    void X(int x);
    void Y(int y);

    int Width() const;
    int Height() const;

    byte Transparency() const;
    void Transparency(byte transparency);

    void AddMeter(Meter *meter);
    void MeterLevels(float value);
    float MeterLevels();

    void BackgroundImage(Gdiplus::Bitmap *background);

private:
    HINSTANCE _hInstance;
    LPCWSTR _className;
    LPCWSTR _title;
    HWND _hWnd;

    POINT _location;
    SIZE _size;
    byte _transparency;

	Gdiplus::Bitmap *_background;
	Gdiplus::Bitmap *_composite;
    RECT *_dirtyRect;

    std::list<Meter*> _meters;

    void UpdateDirtyRect(Gdiplus::Rect &rect);
    void ResetDirtyRect();

    void UpdateLayeredWnd();
    void UpdateLocation();
    void UpdateTransparency();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};