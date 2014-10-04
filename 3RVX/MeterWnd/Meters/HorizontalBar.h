#pragma once
#include "../Meter.h"

class HorizontalBar : public Meter {
public:
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};