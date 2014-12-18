#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include "..\MeterWnd\Meter.h"

class SliderKnob : public Meter {
public:
    SliderKnob(std::wstring bitmapName, int x, int y, int width, int height);
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    Gdiplus::Rect Location();

    int X() const;
    int Y() const;
    void X(int x);
    void Y(int y);
    int Width() const;
    int Height() const;

    int TrackX() const;
    int TrackY() const;
    int TrackWidth() const;
    int TrackHeight() const;

    virtual float Value() const;
    virtual void Value(float value);

private:
    Gdiplus::Rect _track;
};