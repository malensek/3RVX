#include "StaticImage.h"

StaticImage::StaticImage(std::wstring bitmapName, int x, int y) :
Meter(bitmapName, x, y, 1),
_drawRect(X(), Y(), Width(), Height()) {

}

void StaticImage::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _drawRect,
        0, 0, Width(), Height(), Gdiplus::UnitPixel);
}