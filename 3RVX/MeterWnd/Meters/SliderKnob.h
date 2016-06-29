// Copyright (c) 2016, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

class SliderKnob : public Meter {
public:
    SliderKnob(std::wstring bitmapName, int x, int y, int width, int height);

    int TrackX() const;
    int TrackY() const;
    int TrackWidth() const;
    int TrackHeight() const;

    virtual bool Vertical() const = 0;

protected:
    Gdiplus::Rect _track;
};