#pragma once

#include "../Meter.h"
#include <string>

class Text : public Meter {
public:
    Text(int x, int y, int width, int height,
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

    ~Text();

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

protected:
    Gdiplus::Font *_font;
    Gdiplus::SolidBrush *_fontColor;
    Gdiplus::StringFormat _strFormat;
    std::wstring _formatString;
    int _replaceIndex; // index of [[PERC]] string

private:
    virtual void SetBitmap(Gdiplus::Bitmap *meterBitmap) {};
};