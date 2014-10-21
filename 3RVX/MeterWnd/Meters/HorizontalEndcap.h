#pragma once

#include "../Meter.h"

class HorizontalEndcap : public Meter {
public:
    HorizontalEndcap(std::wstring bitmapName, int x, int y, int units):
        Meter(bitmapName, x, y, units),
        _lMargin(-1),
        _rMargin(-1) {

        using namespace Gdiplus;

        Color currentPixel;
        ARGB searchColor = 0xFFFF00FF; /* magic pink (FF,00,FF) */
        unsigned int width = _bitmap->GetWidth();

        /* Scan across, looking for magic pink delineators */
        for (unsigned int x = 0; x < width; ++x) {
            _bitmap->GetPixel(x, 0, &currentPixel);
            if (currentPixel.GetValue() == searchColor) {
                if (_lMargin < 0) {
                    _lMargin = x;
                } else {
                    _rMargin = x;
                    break;
                }
            }
        }

        /* 1 extra px for left margin line */
        _unitWidth = _rMargin - _lMargin - 1;
        _rect.Height = _bitmap->GetHeight();

        _lMarginRect.X = X();
        _lMarginRect.Y = Y();
        _lMarginRect.Width = _lMargin;
        _lMarginRect.Height = Height();

        _rMarginRect.Y = Y();
        _rMarginRect.Width = _bitmap->GetWidth() - _rMargin + 1;
        _rMarginRect.Height = Height();
    };

    void Init();
    void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    void SetBitmap(Gdiplus::Bitmap *meterBitmap);
    void Value(float value);

protected:
    int _lMargin;
    int _rMargin;
    int _unitWidth;

    Gdiplus::Rect _lMarginRect;
    Gdiplus::Rect _rMarginRect;
};