#pragma once

#include "SliderKnob.h"

class HorizontalSlider : public SliderKnob {
public:
    HorizontalSlider(std::wstring bitmapName,
        int x, int y, int width, int height);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    int TrackX() const;
    int TrackY() const;
    int TrackWidth() const;
    int TrackHeight() const;

    virtual float Value() const;
    virtual void Value(float value);
    virtual bool Vertical() const;

protected:
    Gdiplus::Rect _track;
};
