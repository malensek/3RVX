#pragma once

#include <list>
#include "Meter.h"
#include "../LayeredWnd/LayeredWnd.h"

#define TIMER_SHOWANIM    0x01
#define TIMER_HIDEANIM    0x02
#define TIMER_DELAY       0x03

class MeterWnd {
public:
    MeterWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title)
        : m_lWnd(hInstance, className, title) {
        m_lWnd.Init();
    };

	void Update();

    virtual void Show();
    virtual void Hide();

    virtual MeterWnd *Clone
        (HINSTANCE hInstance, LPCWSTR className, LPCWSTR title);

	void SetBackgroundImage(Gdiplus::Bitmap *backgroundImage);
	void SetProperties();
    void SetMeters(float value);
    void AddMeter(Meter *meter);

protected:
    HINSTANCE m_hInstance;
    LPCWSTR m_className;
    LPCWSTR m_title;
    HWND m_hWnd;

    bool                  m_dirty;
    MeterWnd             *m_parent;
	Gdiplus::Bitmap      *m_meterImg;
	Gdiplus::Bitmap      *m_bgImg;
	Gdiplus::Bitmap      *m_wndImg;

    std::list<Meter*>     m_meters;

    void UpdateDirtyRect(Gdiplus::Rect &rect);
    void ResetDirtyRect();

    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
    LayeredWnd m_lWnd;
};