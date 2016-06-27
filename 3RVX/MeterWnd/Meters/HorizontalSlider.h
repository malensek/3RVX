#pragma once

#include "../Meter.h"

class HorizontalSlider : Meter {
public:
    HorizontalSlider(std::wstring bitmapName,
        int x, int y, int width, int height);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    int TrackX() const;
    int TrackY() const;
    int TrackWidth() const;
    int TrackHeight() const;
protected:
    Gdiplus::Rect _track;
};
