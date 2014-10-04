#include "HorizontalBar.h"

void HorizontalBar::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();
    int pixelsPerUnit = Width() / m_units;
    int width = pixelsPerUnit * units;

    if (m_reversed)
        width = Width() - width;

    Gdiplus::Rect drawRect(X(), Y(), width, Height());

    graphics->DrawImage(m_meterBitmap, drawRect,
        0, 0, width, Height(), Gdiplus::UnitPixel);

    m_lastValue = Value();
    m_lastUnits = units;
}