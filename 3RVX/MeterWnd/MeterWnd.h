#pragma once

#include <list>

#include "Animations\AnimationTypes.h"
#include "Meter.h"

class Animation;
class MeterWndClone;

#define TIMER_SHOWANIM    0x01
#define TIMER_HIDEANIM    0x02
#define TIMER_DELAY       0x03

class MeterWnd {
    friend class MeterWndClone;

public:
    MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL,
        AnimationTypes::HideAnimation hideAnim = AnimationTypes::None, 
        int visibleDuration = 0);
    ~MeterWnd();

	void Update();

    MeterWndClone *Clone();

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

    void HideAnimation(AnimationTypes::HideAnimation anim, int speed);
    void VisibleDuration(int duration);

    void BackgroundImage(Gdiplus::Bitmap *background);
    void GlassMask(Gdiplus::Bitmap *mask);

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
	Gdiplus::Bitmap *_glassMask;

    std::list<Meter*> _meters;

    std::vector<MeterWndClone *> _clones;

    int _visibleDuration;
    Animation *_hideAnimation;

    void UpdateDirtyRect(Gdiplus::Rect &rect);
    void ResetDirtyRect();

    void UpdateLayeredWnd();
    void UpdateLocation();
    void UpdateTransparency();
    void ApplyGlass();

    void AnimateOut();
    void AnimateIn();

    void UpdateClones();
    void UpdateClonesTransparency(byte transparency);
    void ShowClones();
    void HideClones();

    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};