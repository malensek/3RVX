#pragma once

#include "../Meter.h"

class Bitstrip : public Meter {
public:
    Bitstrip(std::wstring bitmapName, int x, int y, int units):
        Meter(bitmapName, x, y, units) {

        _rect.Width = _bitmap->GetWidth();
        _rect.Height = _bitmap->GetHeight() / _units;
    }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};