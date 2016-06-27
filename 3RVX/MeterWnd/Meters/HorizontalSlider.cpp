#include "HorizontalSlider.h"

HorizontalSlider::HorizontalSlider(
    std::wstring bitmapName, int x, int y, int width, int height) :
SliderKnob(bitmapName, x, y, width, height) {
    _units = _track.Width - _rect.Width;
}

void HorizontalSlider::Draw(
        Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);
}

float HorizontalSlider::Value() const {
    int xPos = X() - TrackX();
    int xMax = TrackWidth() - _rect.Width;
    return (float) xPos / (float) xMax;
}

void HorizontalSlider::Value(float value) {
    Meter::Value(value);
    X(TrackX() + CalcUnits());
}

bool HorizontalSlider::Vertical() const {
    return false;
}
