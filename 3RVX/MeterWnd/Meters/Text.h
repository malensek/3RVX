#pragma once

#include "../Meter.h"
#include <string>

class Text : public Meter {
public:
    Text(int x, int y, int width, int height,
        Gdiplus::Font *font, Gdiplus::StringAlignment align,
        std::wstring color, byte transparency,
        std::wstring formatString);
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