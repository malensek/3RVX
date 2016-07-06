// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

class HorizontalTile : public Meter {
public:
    HorizontalTile(std::wstring bitmapName,
        int x, int y, int units, bool reverse = false);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    ~HorizontalTile();

protected:
    Gdiplus::TextureBrush *_texture;
    Gdiplus::Matrix *_shiftMat;
    bool _reverse;
};