#pragma once

#include "HorizontalTile.h"

class VerticalTile : public HorizontalTile {
public:
    VerticalTile(std::wstring bitmapName, int x, int y, int units) :
    HorizontalTile(bitmapName, x, y, units) { }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};