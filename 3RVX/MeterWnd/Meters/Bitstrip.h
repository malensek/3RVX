// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

class Bitstrip : public Meter {
public:
    Bitstrip(std::wstring bitmapName, int x, int y, int units);
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};