// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

class StaticImage : public Meter {
public:
    StaticImage(std::wstring bitmapName, int x, int y);
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

private:
    Gdiplus::Rect _drawRect;
};