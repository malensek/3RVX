#include "Meter.h"
#include <math.h>
#include <sstream>

Meter::Meter(std::wstring bitmapName, int x, int y, int units) :
_value(0.0f),
_lastValue(-1.0f),
_units(units),
_lastUnits(-1) {
    _rect.X = x;
    _rect.Y = y;

    Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(
        bitmapName.c_str(), false);
    CLOG(L"Loading meter bitmap: %s\nStatus: %d",
        bitmapName.c_str(), bmp->GetLastStatus());
    _bitmap = bmp;

    _rect.Width = bmp->GetWidth();
    _rect.Height = bmp->GetHeight();
}

Meter::Meter(int x, int y, int units) :
_bitmap(NULL),
_value(0.0f),
_lastValue(-1.0f),
_units(units),
_lastUnits(-1) {
    _rect.X = x;
    _rect.Y = y;
}

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
    return (int) ceil(Value() * _units);
}

std::wstring Meter::ToString() {
    std::wstringstream ss;
    ss << L"Geometry: (" << X() << L", " << Y() << "); ";
    ss << Width() << L"x" << Height() << std::endl;
    ss << L"Units: " << Units() << std::endl;
    ss << L"Current Value: " << Value() << L" (" << CalcUnits() << L" units)";
    return ss.str();
}