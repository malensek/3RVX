#include "Bitstrip.h"

void Bitstrip::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();
    int stripY = (units - 1) * _rect.Height;

    if (units == 0) {
        /* The mute OSD should be shown here, but we'll do something sane
         * rather than go negative. */
        stripY = 0;
    }

    Gdiplus::Rect drawRect(X(), Y(), Width(), Height());

    graphics->DrawImage(_bitmap, drawRect,
        0, stripY, Width(), Height(), Gdiplus::UnitPixel);

    _lastValue = Value();
    _lastUnits = units;
}