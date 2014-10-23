#include "HorizontalEndcap.h"

void
HorizontalEndcap::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();

    Gdiplus::Rect destRect(X(), Y(), _lMarginRect.Width, Height());

    /* draw the left endcap */
    graphics->DrawImage(_bitmap, destRect,
        0, 0, _lMargin, Height(), Gdiplus::UnitPixel);

    /* draw the middle of the bar */
    destRect.X = destRect.X + _lMargin;
    destRect.Width = _unitWidth * units;

    Gdiplus::TextureBrush tB(_bitmap, Gdiplus::WrapModeTile,
        _lMargin + 1, 0, _rMargin - _lMargin - 1, Height());

    /* translate the texture brush into position.
     * note, 1st param = how much X stretch, 4th = how much Y stretch */
    Gdiplus::Matrix mat(1, 0, 0, 1, (float) destRect.X, (float) destRect.Y);
    tB.SetTransform(&mat);

    graphics->FillRectangle(&tB, destRect);

    /* draw the right endcap */
    destRect.X = destRect.X + destRect.Width;
    destRect.Width = _rMarginRect.Width;
    graphics->DrawImage(_bitmap, destRect,
        _rMargin + 1, 0, _rMarginRect.Width, Height(), Gdiplus::UnitPixel);

    _lastValue = Meter::Value();
    _lastUnits = units;
}

void HorizontalEndcap::Value(float value) {
    Meter::Value(value);
    _rect.Width = _lMargin + _unitWidth * CalcUnits() + _rMargin;
}