#include "VerticalSliderKnob.h"

VerticalSliderKnob::VerticalSliderKnob(
    std::wstring bitmapName, int x, int y, int width, int height) :
SliderKnob(bitmapName, x, y, width, height) {
    _units = _track.Height - _rect.Height;
    _rect.X = _track.X + (_track.Width / 2) - (_rect.Width / 2);
}

void VerticalSliderKnob::Draw(
        Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);
}

float VerticalSliderKnob::Value() const {
    int yPos = Y() - TrackY();
    int yMax = TrackHeight() - _rect.Height;
    return 1.0f - (float) yPos / (float) yMax;
}

void VerticalSliderKnob::Value(float value) {
    Meter::Value(value);
    Y(TrackY() + _units - CalcUnits());
}

bool VerticalSliderKnob::Vertical() const {
    return true;
}