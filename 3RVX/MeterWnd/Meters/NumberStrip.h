// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <string>

#include "../Meter.h"

class NumberStrip : public Meter {
public:
    NumberStrip(std::wstring bitmapName, int x, int y, int units,
        Gdiplus::StringAlignment align);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    virtual std::wstring ToString();

private:
    Gdiplus::StringAlignment _align;
    int _charWidth;
};