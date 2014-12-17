#include "SliderKnob.h"

SliderKnob::SliderKnob(std::wstring bitmapName,
    int x, int y, int width, int height) :
Meter(bitmapName, x, y, 1),
_trackX(x),
_trackY(y),
_trackWidth(width),
_trackHeight(height) {
    _units = _trackWidth - Width();
}

Gdiplus::Rect SliderKnob::Location() {
    return _rect;
}

void SliderKnob::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, Width(), _trackHeight, Gdiplus::UnitPixel);
}

void SliderKnob::Value(float value) {
    Meter::Value(value);
    X(_trackX + CalcUnits());
}

int SliderKnob::TrackX() const {
    return _trackX;
}

int SliderKnob::TrackY() const {
    return _trackY;
}

int SliderKnob::TrackWidth() const {
    return _trackWidth;
}

int SliderKnob::TrackHeight() const {
    return _trackHeight;
}

int SliderKnob::X() const {
    return Meter::X();
}

int SliderKnob::Y() const {
    return Meter::Y();
}

void SliderKnob::X(int x) {
    _rect.X = x;
}

void SliderKnob::Y(int y) {
    _rect.Y = y;
}