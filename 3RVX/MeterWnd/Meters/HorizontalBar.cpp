#include "HorizontalBar.h"

void HorizontalBar::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int currentUnits = CalcUnits();
    int pixelsPerUnit = Width() / _units;
    int width = pixelsPerUnit * currentUnits;

    if (_reversed) {
        width = Width() - width;
    }

    Gdiplus::Rect drawRect(X(), Y(), width, Height());

    graphics->DrawImage(_bitmap, drawRect,
        0, 0, width, Height(), Gdiplus::UnitPixel);

    _lastValue = Value();
    _lastUnits = currentUnits;
}