#include "VerticalTile.h"

void VerticalTile::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int currentUnits = CalcUnits();
    int height = Height() * currentUnits;

    graphics->FillRectangle(_texture, X(), Y(), Width(), height);

    _lastValue = Value();
    _lastUnits = currentUnits;
}