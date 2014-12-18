#include "Text.h"
#include <algorithm>

Text::Text(int x, int y, int width, int height,
        Gdiplus::Font *font, Gdiplus::StringAlignment align,
        std::wstring color, byte transparency,
        std::wstring formatString) :
Meter(x, y, 100),
_font(font->Clone()),
_formatString(formatString) {
    _rect.Width = width;
    _rect.Height = height;

    _strFormat.SetAlignment(align);

    unsigned long c = wcstol(color.c_str(), '\0', 16);
    unsigned long a = transparency << 24;
    _fontColor = new Gdiplus::SolidBrush(c | a);

    _replaceIndex = _formatString.find(L"[[PERC]]");
}

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

    Gdiplus::RectF layoutRect((float) _rect.X, (float) _rect.Y, 
        (float) _rect.Width, (float) _rect.Height);

    graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    wchar_t perc[4];
    _itow_s(units * (100 / _units), perc, 10);
    std::wstring tempstr(_formatString);
    const wchar_t *str = tempstr.replace(_replaceIndex, 8, perc).c_str();

    graphics->DrawString(str, -1, _font, layoutRect, 
        &_strFormat, _fontColor);

    UpdateDrawnValues();
}