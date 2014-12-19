#include "SliderKnob.h"

SliderKnob::SliderKnob(std::wstring bitmapName,
    int x, int y, int width, int height, bool vertical) :
Meter(bitmapName, x, y, 1),
_track(x, y, width, height),
_vertical(vertical) {

    if (_vertical) {
        _units = _track.Height - _rect.Height;
        _rect.X = _track.X + (_track.Width / 2) - (_rect.Width / 2);
    } else {
        _units = _track.Width - _rect.Width;
    }

}

void SliderKnob::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);
}

float SliderKnob::Value() const {
    if (_vertical) {
        int yPos = Y() - TrackY();
        int yMax = TrackHeight() - _rect.Height;
        return 1.0f - (float) yPos / (float) yMax;
    } else {
        int xPos = X() - TrackX();
        int xMax = TrackWidth() - _rect.Width;
        return (float) xPos / (float) xMax;
    }
}

void SliderKnob::Value(float value) {
    Meter::Value(value);
    if (_vertical) {
        Y(TrackY() + _units - CalcUnits());
    } else {
        X(TrackX() + CalcUnits());
    }
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

bool SliderKnob::Vertical() const {
    return _vertical;
}