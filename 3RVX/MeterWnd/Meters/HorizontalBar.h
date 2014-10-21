#pragma once
#include "../Meter.h"

class HorizontalBar : public Meter {
public:
    HorizontalBar(std::wstring bitmapName, int x, int y, int units,
        bool reversed):
        Meter(bitmapName, x, y, units),
        _reversed(reversed) {
    };

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
private:
    bool _reversed = false;
};