#include "SliderKnob.h"

SliderKnob::SliderKnob(std::wstring bitmapName,
    int x, int y, int width, int height) :
Meter(bitmapName, x, y, 1),
_track(x, y, width, height) {
    _units = _track.Width - _rect.Width;
}

Gdiplus::Rect SliderKnob::Location() {
    return _rect;
}

void SliderKnob::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _track.Height, Gdiplus::UnitPixel);
}

float SliderKnob::Value() const {
    int xPos = X() - TrackX();
    int xMax = TrackWidth() - _rect.Width;
    return (float) xPos / (float) xMax;
}

void SliderKnob::Value(float value) {
    Meter::Value(value);
    X(TrackX() + CalcUnits());
}

int SliderKnob::TrackX() const {
    return _track.X;
}

int SliderKnob::TrackY() const {
    return _track.Y;
}

int SliderKnob::TrackWidth() const {
    return _track.Width;
}

int SliderKnob::TrackHeight() const {
    return _track.Height;
}

int SliderKnob::X() const {
    return _rect.X;
}

int SliderKnob::Y() const {
    return _rect.Y;
}

void SliderKnob::X(int x) {
    _rect.X = x;
}

void SliderKnob::Y(int y) {
    _rect.Y = y;
}

int SliderKnob::Width() const {
    return _rect.Width;
}

int SliderKnob::Height() const {
    return _rect.Height;
}