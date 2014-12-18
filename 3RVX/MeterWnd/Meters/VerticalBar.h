#pragma once

#include "../Meter.h"

class VerticalBar : public Meter {
public:
    VerticalBar(std::wstring bitmapName, int x, int y,
        int units, bool reversed = false);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

private:
    int _pixelsPerUnit;
    bool _reversed;
};