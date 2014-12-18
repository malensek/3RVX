#include "HorizontalTile.h"

HorizontalTile::~HorizontalTile()
{
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