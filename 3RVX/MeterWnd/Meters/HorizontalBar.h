#pragma once
#include "../Meter.h"

class HorizontalBar : public Meter {
public:
    HorizontalBar(std::wstring bitmapName, int x, int y, int units,
        bool reversed = false):
        Meter(bitmapName, x, y, units),
        _reversed(reversed) {
        _rect.Width = _bitmap->GetWidth();
        _rect.Height = _bitmap->GetHeight();
    };

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

private:
    bool _reversed;
};