#pragma once

#include "../Meter.h"

class HorizontalEndcap : public Meter {
public:
    HorizontalEndcap(std::wstring bitmapName, int x, int y, int units);
    void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    void Value(float value);

protected:
    int _lMargin;
    int _rMargin;
    int _unitWidth;

    Gdiplus::Rect _lMarginRect;
    Gdiplus::Rect _rMarginRect;
};