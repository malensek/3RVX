#pragma once

#include "../Meter.h"

class HorizontalSlider : Meter {
    HorizontalSlider(std::wstring bitmapName, int x, int y, int units);
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};
