#include "HorizontalBar.h"

void HorizontalBar::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int pixelsPerUnit = _rect.Width / _units;
    int width = pixelsPerUnit * CalcUnits();

    if (_reversed) {
        width = _rect.Width - width;
    }

    Gdiplus::Rect drawRect(_rect.X, _rect.Y, width, _rect.Height);

    graphics->DrawImage(_bitmap, drawRect,
        0, 0, width, _rect.Height, Gdiplus::UnitPixel);

    UpdateDrawnValues();
}