#include "HorizontalTile.h"

HorizontalTile::~HorizontalTile()
{
    delete _texture;
    delete _shiftMat;
}

void HorizontalTile::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int currentUnits = CalcUnits();
    int width = Width() * currentUnits;

    if (_reverse == false) {
        graphics->FillRectangle(_texture, X(), Y(), width, Height());
    } else {
        int xShift = Width() * Units() - width;
        graphics->FillRectangle(_texture, X() + xShift, Y(), width, Height());
    }

    _lastValue = Value();
    _lastUnits = currentUnits;
}