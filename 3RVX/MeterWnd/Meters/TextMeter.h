#pragma once

#include "../Meter.h"
#include <string>

class TextMeter : public Meter {
public:
    TextMeter(int x, int y, int width, int height,
        std::wstring fontName, float fontSize, int fontStyle,
        TextAlignment align, std::wstring color, byte transparency,
        std::wstring formatString) :
    Meter(x, y, 100),
    _formatString(formatString) {
        _rect.Width = width;
        _rect.Height = height;

        _font = new Gdiplus::Font(fontName.c_str(), fontSize, fontStyle);

        _strFormat.SetAlignment(Gdiplus::StringAlignmentNear);
        if (align == Right) {
            _strFormat.SetAlignment(Gdiplus::StringAlignmentFar);
        } else if (align == Center) {
            _strFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
        }

        unsigned long c = wcstol(color.c_str(), '\0', 16);
        unsigned long a = transparency << 24;
        _fontColor = new Gdiplus::SolidBrush(c | a);
    }

    ~TextMeter();

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

protected:
    Gdiplus::Font *_font;
    Gdiplus::SolidBrush *_fontColor;
    Gdiplus::StringFormat _strFormat;
    std::wstring _formatString;
    int m_repl; // index of [[PERC]] string

private:
    virtual void SetBitmap(Gdiplus::Bitmap *meterBitmap) {};
};