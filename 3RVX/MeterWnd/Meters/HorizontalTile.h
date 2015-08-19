// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

class HorizontalTile : public Meter {
public:
    HorizontalTile(std::wstring bitmapName, int x, int y, int units,
        bool reverse = false) :
    Meter(bitmapName, x, y, units),
    _reverse(reverse) {
        _rect.Width = _bitmap->GetWidth();
        _rect.Height = _bitmap->GetHeight();

        _texture = new Gdiplus::TextureBrush(_bitmap, Gdiplus::WrapModeTile,
            0, 0, _rect.Width, _rect.Height);
        _shiftMat = new Gdiplus::Matrix(1, 0, 0, 1,
            (float) _rect.X, (float) _rect.Y);
        _texture->SetTransform(_shiftMat);
    }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    ~HorizontalTile();

protected:
    Gdiplus::TextureBrush *_texture;
    Gdiplus::Matrix *_shiftMat;
    bool _reverse;
};