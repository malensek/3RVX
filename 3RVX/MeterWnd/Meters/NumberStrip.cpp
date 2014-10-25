#include "NumberStrip.h"
#include <algorithm>

void NumberStrip::Init()
{
    m_charHeight = Height() / 10; // 10 numbers 0-9

}

void NumberStrip::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int units = CalcUnits();
    int perc = units * (100 / m_units);

    Gdiplus::Rect drawRect(X(), Y(), Width(), m_charHeight);

    graphics->DrawImage(m_meterBitmap, drawRect,
        0, 0, Width(), m_charHeight, Gdiplus::UnitPixel);

    m_lastValue = Value();
    m_lastUnits = units;
}

void NumberStrip::SetAlignment(const wchar_t *alignment)
{
    std::wstring align(alignment);
    std::transform(align.begin(), align.end(), 
        align.begin(), (int(*)(int)) toupper);

    if(align == L"LEFT")
        m_align = NUMBERSTRIP_ALIGN_LEFT;
    else if(align == L"RIGHT")
        m_align = NUMBERSTRIP_ALIGN_RIGHT;
    else
        m_align = NUMBERSTRIP_ALIGN_CENTER;
}

void NumberStrip::SetColumns(byte cols)
{
    if(cols > 3)
        cols = 3;

    if(cols < 1)
        cols = 1;

    m_cols = cols;
}