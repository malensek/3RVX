#include "Text.h"
#include <algorithm>

Text::~Text()
{
    if (_font) {
        delete _font;
    }

    if (_fontColor) {
        delete _fontColor;
    }
}

void Text::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int units = CalcUnits();

    Gdiplus::RectF layoutRect((float)X(), (float)Y(), 
        (float)Width(), (float)Height());

    graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    wchar_t perc[4];
    _itow_s(units * (100 / _units), perc, 10);
    std::wstring tempstr(_formatString);
    const wchar_t *str = tempstr.replace(_replaceIndex, 8, perc).c_str();

    graphics->DrawString(str, -1, _font, layoutRect, 
        &_strFormat, _fontColor);

    _lastValue = Value();
    _lastUnits = units;
}