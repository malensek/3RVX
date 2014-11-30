#pragma once

#include <list>
#include "Meter.h"

#define TIMER_SHOWANIM    0x01
#define TIMER_HIDEANIM    0x02
#define TIMER_DELAY       0x03

class MeterWnd {
public:
    MeterWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title,
        Animation *hideAnimation = NULL, Animation *showAnimation = NULL,
        int visibleDuration = 0);

	void Update();

    void Show(bool animate = true);
    void Hide(bool animate = true);

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
    LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};