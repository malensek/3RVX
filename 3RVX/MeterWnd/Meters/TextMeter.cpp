#include "TextMeter.h"
#include <algorithm>

TextMeter::~TextMeter()
{
    if (_font) {
        delete _font;
    }

    if (_fontColor) {
        delete _fontColor;
    }
}

void TextMeter::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics)
{
    int units = CalcUnits();

    Gdiplus::RectF layoutRect((float)X(), (float)Y(), 
        (float)Width(), (float)Height());

    graphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

    wchar_t perc[4];
    _itow_s(units * (100 / _units), perc, 10);
    std::wstring tempstr(_formatString);
    const wchar_t *str = tempstr.replace(m_repl, 8, perc).c_str();

    graphics->DrawString(str, -1, _font, layoutRect, 
        &_strFormat, _fontColor);

    _lastValue = Value();
    _lastUnits = units;
}

/*
std::wstring style(fontStyle);
std::transform(style.begin(), style.end(), 
style.begin(), (int(*)(int)) toupper);

int s = 0; // font style flags
std::wstring::size_type sz;

sz = style.find(L"BOLD", 0);
if(sz != std::wstring::npos)
s = s | Gdiplus::FontStyleBold;
    
sz = style.find(L"ITALIC", 0);
if(sz != std::wstring::npos)
s = s | Gdiplus::FontStyleItalic;

sz = style.find(L"UNDERLINE", 0);
if(sz != std::wstring::npos)
s = s | Gdiplus::FontStyleUnderline;

sz = style.find(L"STRIKE", 0);
if(sz != std::wstring::npos)
s = s | Gdiplus::FontStyleStrikeout;

if(m_font)
delete m_font;

m_font = new Gdiplus::Font(fontName, size, s, Gdiplus::UnitPixel);
*/

    /*
    m_formatString = str;
    m_repl = (int)m_formatString.find(L"[[PERC]]", 0);
    */
    /*
    unsigned long c = wcstol(hexColor, '\0', 16);
    unsigned long a = transparency << 24;

    if(m_fontColor)
        delete m_fontColor;

    Gdiplus::ARGB color = c | a;
    _fontColor = new Gdiplus::SolidBrush(color);
    */

    /*
    std::wstring align(alignment);
    std::transform(align.begin(), align.end(), 
        align.begin(), (int(*)(int)) toupper);

    if(align == L"LEFT")
        m_strFormat.SetAlignment(Gdiplus::StringAlignmentNear);
    else if(align == L"RIGHT")
        m_strFormat.SetAlignment(Gdiplus::StringAlignmentFar);
    else
        m_strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);

    m_strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    m_strFormat.SetFormatFlags(Gdiplus::StringFormatFlagsNoFontFallback);
    */