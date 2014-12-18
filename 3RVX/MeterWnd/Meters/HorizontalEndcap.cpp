#include "HorizontalEndcap.h"

HorizontalEndcap::HorizontalEndcap(
    std::wstring bitmapName, int x, int y, int units) :
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

    _lMarginRect.X = _rect.X;
    _lMarginRect.Y = _rect.Y;
    _lMarginRect.Width = _lMargin;
    _lMarginRect.Height = _rect.Height;

    _rMarginRect.Y = _rect.Y;
    _rMarginRect.Width = _bitmap->GetWidth() - _rMargin + 1;
    _rMarginRect.Height = _rect.Height;
}

void
HorizontalEndcap::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {

    Gdiplus::Rect destRect(_rect.X, _rect.Y, _lMarginRect.Width, _rect.Height);

    /* draw the left endcap */
    graphics->DrawImage(_bitmap, destRect,
        0, 0, _lMargin, _rect.Height, Gdiplus::UnitPixel);

    /* draw the middle of the bar */
    destRect.X = destRect.X + _lMargin;
    destRect.Width = _unitWidth * CalcUnits();

    Gdiplus::TextureBrush tB(_bitmap, Gdiplus::WrapModeTile,
        _lMargin + 1, 0, _rMargin - _lMargin - 1, _rect.Height);

    /* translate the texture brush into position.
     * note, 1st param = how much X stretch, 4th = how much Y stretch */
    Gdiplus::Matrix mat(1, 0, 0, 1, (float) destRect.X, (float) destRect.Y);
    tB.SetTransform(&mat);

    graphics->FillRectangle(&tB, destRect);

    /* draw the right endcap */
    destRect.X = destRect.X + destRect.Width;
    destRect.Width = _rMarginRect.Width;
    graphics->DrawImage(_bitmap, destRect,
        _rMargin + 1, 0, _rMarginRect.Width, _rect.Height, Gdiplus::UnitPixel);

    UpdateDrawnValues();
}

void HorizontalEndcap::Value(float value) {
    Meter::Value(value);
    _rect.Width = _lMargin + _unitWidth * CalcUnits() + _rMargin;
}