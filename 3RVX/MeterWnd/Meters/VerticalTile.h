// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "HorizontalTile.h"

class VerticalTile : public HorizontalTile {
public:
    VerticalTile(std::wstring bitmapName, int x, int y, int units) :
    HorizontalTile(bitmapName, x, y, units) { }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};