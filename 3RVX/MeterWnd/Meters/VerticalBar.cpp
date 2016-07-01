// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "VerticalBar.h"

VerticalBar::VerticalBar(std::wstring bitmapName, int x, int y,
    int units, bool reversed) :
Meter(bitmapName, x, y, units),
_pixelsPerUnit(_rect.Height / _units),
_reversed(reversed) {

}

void VerticalBar::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int height = _pixelsPerUnit * CalcUnits();
    int yOffset = _rect.Y + _rect.Height - height;

    Gdiplus::Rect drawRect(_rect.X, yOffset, _rect.Width, height);
    if (_reversed) {
        drawRect.Y = _rect.Y;
    }

    Gdiplus::ImageAttributes ia;
    Gdiplus::ColorMap blackToRed;
    blackToRed.oldColor = Gdiplus::Color(255, 255, 0, 255);  // black
    blackToRed.newColor = Gdiplus::Color(255, 255, 0, 0);// red
    ia.SetRemapTable(1, &blackToRed);


    graphics->DrawImage(_bitmap, drawRect,
        0, 0, _rect.Width, height, Gdiplus::UnitPixel, &ia, NULL, NULL);

    UpdateDrawnValues();
}