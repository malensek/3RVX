#pragma once

#include <string>

#include "../Meter.h"

class NumberStrip : public Meter {
public:

    NumberStrip(std::wstring bitmapName, int x, int y, int units,
        Gdiplus::StringAlignment align):
    Meter(bitmapName, x, y, units),
    _align(align) {
        _charWidth = _bitmap->GetWidth();

        _rect.Width = _bitmap->GetWidth() * 3;
        _rect.Height = _bitmap->GetHeight() / 10;
    }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    virtual std::wstring ToString();

private:
    Gdiplus::StringAlignment _align;
    int _charWidth;
};