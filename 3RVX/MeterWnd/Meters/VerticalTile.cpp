// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "VerticalTile.h"

void VerticalTile::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int currentUnits = CalcUnits();
    int height = _rect.Height * currentUnits;

    graphics->FillRectangle(_texture, _rect.X, _rect.Y, _rect.Width, height);

    UpdateDrawnValues();
}