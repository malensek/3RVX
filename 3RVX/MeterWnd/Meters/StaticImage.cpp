#include "StaticImage.h"

StaticImage::StaticImage(std::wstring bitmapName, int x, int y) :
Meter(bitmapName, x, y, 1) {

}

void StaticImage::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);

    UpdateDrawnValues();
}