// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "OldSliderKnob.h"

OldSliderKnob::OldSliderKnob(std::wstring bitmapName,
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

void OldSliderKnob::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    graphics->DrawImage(_bitmap, _rect,
        0, 0, _rect.Width, _rect.Height, Gdiplus::UnitPixel);
}

float OldSliderKnob::Value() const {
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

void OldSliderKnob::Value(float value) {
    Meter::Value(value);
    if (_vertical) {
        Y(TrackY() + _units - CalcUnits());
    } else {
        X(TrackX() + CalcUnits());
    }
}

int OldSliderKnob::TrackX() const {
    return _track.X;
}

int OldSliderKnob::TrackY() const {
    return _track.Y;
}

int OldSliderKnob::TrackWidth() const {
    return _track.Width;
}

int OldSliderKnob::TrackHeight() const {
    return _track.Height;
}

bool OldSliderKnob::Vertical() const {
    return _vertical;
}