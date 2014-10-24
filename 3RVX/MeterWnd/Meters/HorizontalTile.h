#pragma once

#include "../Meter.h"

class HorizontalTile : public Meter {
public:
    HorizontalTile(std::wstring bitmapName, int x, int y, int units):
    Meter(bitmapName, x, y, units) {
        _rect.Width = _bitmap->GetWidth();
        _rect.Height = _bitmap->GetHeight();

        _texture = new Gdiplus::TextureBrush(_bitmap, Gdiplus::WrapModeTile,
            0, 0, Width(), Height());
        _shiftMat = new Gdiplus::Matrix(1, 0, 0, 1, (float) X(), (float) Y());
        _texture->SetTransform(_shiftMat);
    }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    ~HorizontalTile();

private:
    Gdiplus::TextureBrush *_texture;
    Gdiplus::Matrix *_shiftMat;
};