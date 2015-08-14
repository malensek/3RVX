#pragma once

#include <list>

#include "Animations\AnimationTypes.h"
#include "LayeredWnd.h"
#include "Meter.h"

class Animation;

class MeterWnd : public LayeredWnd {
public:
    MeterWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL);
    ~MeterWnd();

	void Update();

    LayeredWnd *Clone();
    std::vector<LayeredWnd *> Clones();

    virtual void Show(bool animate = true);
    virtual void Hide(bool animate = true);

    byte Transparency();
    void Transparency(byte transparency);

    void AddMeter(Meter *meter);
    void MeterLevels(float value);
    float MeterLevels();

    void HideAnimation(AnimationTypes::HideAnimation anim, int speed);
    void VisibleDuration(int duration);

    /// <summary>
    /// Reports whether this meter window displays when a full-screen
    /// application is focused.
    /// </summary>
    bool DisableFullscreen();
    /// <summary>
    /// Sets whether this meter window should display when a full-screen
    /// application is focused.
    /// </summary>
    void DisableFullscreen(bool disable);

    /// <summary>
    /// Reports whether this meter window displays when a DirectX application
    /// is focused and is occluding the screen. Basically, when a game or
    /// application has exclusive control of the display.
    /// </summary>
    bool DisableDirectX();
    /// <summary>
    /// Sets whether this meter window should display when a DirectX application
    /// application is occluding the screen.
    /// </summary>
    void DisableDirectX(bool disable);

    void BackgroundImage(Gdiplus::Bitmap *background);
    bool EnableGlass(Gdiplus::Bitmap *mask);

protected:
    /// <summary>
    /// The composite (drawn) image for this window, including the background
    /// and meter states.
    /// </summary>
	Gdiplus::Bitmap *_composite;

    Gdiplus::Bitmap *_background;

    RECT *_dirtyRect;

    std::list<Meter*> _meters;
    std::vector<LayeredWnd *> _clones;

    int _visibleDuration;
    Animation *_hideAnimation;

    bool _disableFullscreen;
    bool _disableDirectX;

    void UpdateLocation();
    void UpdateTransparency();
    void ApplyGlass();

    void AnimateOut();
    void AnimateIn();

    void UpdateClones();
    void UpdateClonesTransparency(byte transparency);
    void ShowClones();
    void HideClones();
    void ApplyClonesGlass();

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

private:
    /// <summary>Extended window styles for Meter windows.</summary>
    static const DWORD WINDOW_STYLES
        = WS_EX_TOOLWINDOW
        | WS_EX_NOACTIVATE
        | WS_EX_TOPMOST
        | WS_EX_TRANSPARENT;

    /// <summary>
    /// Timer ID used for determining when the window display duration has
    /// elapsed.
    /// <summary>
    static const int TIMER_HIDE = 100;

    /// <summary> Timer ID used to animate the window as it is hidden.</summary>
    static const int TIMER_OUT = 101;
};