#include "HorizontalSliderKnob.h"

HorizontalSliderKnob::HorizontalSliderKnob(
    std::wstring bitmapName, int x, int y, int width, int height) :
SliderKnob(bitmapName, x, y, width, height) {
    _units = _track.Width - _rect.Width;
}

void HorizontalSliderKnob::Draw(
        Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);
}

float HorizontalSliderKnob::Value() const {
    int xPos = X() - TrackX();
    int xMax = TrackWidth() - _rect.Width;
    return (float) xPos / (float) xMax;
}

void HorizontalSliderKnob::Value(float value) {
    Meter::Value(value);
    X(TrackX() + CalcUnits());
}

bool HorizontalSliderKnob::Vertical() const {
    return false;
}
