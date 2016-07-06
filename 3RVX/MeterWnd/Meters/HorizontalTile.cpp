// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "HorizontalTile.h"

HorizontalTile::HorizontalTile(std::wstring bitmapName,
    int x, int y, int units, bool reverse) :
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


HorizontalTile::~HorizontalTile() {
    delete _texture;
    delete _shiftMat;
}

void HorizontalTile::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int width = _rect.Width * CalcUnits();

    if (_reverse == false) {
        graphics->FillRectangle(_texture,
            _rect.X, _rect.Y, width, _rect.Height);
    } else {
        int xShift = _rect.Width * Units() - width;
        graphics->FillRectangle(_texture,
            _rect.X + xShift, _rect.Y, width, _rect.Height);
    }

    UpdateDrawnValues();
}