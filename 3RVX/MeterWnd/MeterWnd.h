#pragma once

#include <list>
#include "Meter.h"

class Animation;

#define TIMER_SHOWANIM    0x01
#define TIMER_HIDEANIM    0x02
#define TIMER_DELAY       0x03

class MeterWnd {
public:
    MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL,
        Animation *hideAnimation = NULL, Animation *showAnimation = NULL,
        int visibleDuration = 0);
    ~MeterWnd();

	void Update();

    virtual void Show(bool animate = true);
    virtual void Hide(bool animate = true);

    int X() const;
    int Y() const;
    void X(int x);
    void Y(int y);

    int Width() const;
    int Height() const;

    void Move(int x, int y);

    byte Transparency() const;
    void Transparency(byte transparency);

    void AddMeter(Meter *meter);
    void MeterLevels(float value);
    float MeterLevels();

    void HideAnimation(Animation *anim);
    void ShowAnimation(Animation *anim);
    void VisibleDuration(int duration);

    void BackgroundImage(Gdiplus::Bitmap *background);

protected:
    HINSTANCE _hInstance;
    LPCWSTR _className;
    LPCWSTR _title;
    HWND _hWnd;

    POINT _location;
    SIZE _size;
    byte _transparency;
    bool _visible;

	Gdiplus::Bitmap *_background;
	Gdiplus::Bitmap *_composite;
    RECT *_dirtyRect;

    std::list<Meter*> _meters;

    int _visibleDuration;
    Animation *_hideAnimation;
    Animation *_showAnimation;

    void UpdateDirtyRect(Gdiplus::Rect &rect);
    void ResetDirtyRect();

    void UpdateLayeredWnd();
    void UpdateLocation();
    void UpdateTransparency();

    void AnimateOut();
    void AnimateIn();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};