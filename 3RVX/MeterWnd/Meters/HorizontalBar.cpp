// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "HorizontalBar.h"

HorizontalBar::HorizontalBar(std::wstring bitmapName, int x, int y,
    int units, bool reversed) :
Meter(bitmapName, x, y, units),
_pixelsPerUnit(_rect.Width / _units),
_reversed(reversed) {

}

void HorizontalBar::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int width = _pixelsPerUnit * CalcUnits();

    if (_reversed) {
        width = _rect.Width - width;
    }

    Gdiplus::Rect drawRect(_rect.X, _rect.Y, width, _rect.Height);

    graphics->DrawImage(_bitmap, drawRect, 0, 0, width, _rect.Height,
        Gdiplus::UnitPixel, &_imageAttributes, NULL, NULL);

    UpdateDrawnValues();
}