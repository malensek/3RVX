#include "Meter.h"

float Meter::Value() {
    return _value;
}

void Meter::Value(float value) {
    if (value > 1.0) { value = 1.0; }
    if (value < 0) { value = 0; }

    _value = value;
}

int Meter::Units() {
    return _units;
}

Gdiplus::Rect Meter::Rect() const {
    return _rect;
}

int Meter::X() const {
    return _rect.X;
}

int Meter::Y() const {
    return _rect.Y;
}

int Meter::Width() const {
    return _rect.Width;
}

int Meter::Height() const {
    return _rect.Height;
}

bool Meter::Dirty() {
    /* Not dirty if the meter's value is unchanged */
    if (Value() == _lastValue) {
        return false;
    }

    /* Not dirty if the change in value won't affect the meter graphically */
    if (CalcUnits() == _lastUnits) {
        return false;
    }

    return true;
}

int Meter::CalcUnits() {
    return (int) (Value() * (float) _units + 0.5);
}