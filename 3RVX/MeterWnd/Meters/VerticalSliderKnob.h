#pragma once

#include "SliderKnob.h"

class VerticalSliderKnob : public SliderKnob {
public:
    VerticalSliderKnob(std::wstring bitmapName,
        int x, int y, int width, int height);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    virtual float Value() const;
    virtual void Value(float value);
    virtual bool Vertical() const;
};