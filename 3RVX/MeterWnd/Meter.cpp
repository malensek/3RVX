#include "Meter.h"

void Meter::Init() {}

float Meter::Value() {
    return m_meterValue;
}

void Meter::SetValue(float value) {
    if (value > 1.0) { value = 1.0; }
    if (value < 0) { value = 0; }

    m_meterValue = value;
}

void Meter::SetLocation(int x, int y) {
    m_rect.X = x;
    m_rect.Y = y;
}

void Meter::SetSize(int width, int height) {
    m_rect.Width = width;
    m_rect.Height = height;
}

void Meter::SetReverse(bool reversed) {
    m_reversed = reversed;
}

bool Meter::Reversed() {
    return m_reversed;
}

void Meter::SetUnits(int units) {
    m_units = units;
}

int Meter::Units() {
    return m_units;
}

Gdiplus::Rect Meter::Rect() {
    return m_rect;
}

int Meter::X() {
    return m_rect.X;
}

int Meter::Y() {
    return m_rect.Y;
}

int Meter::Width() {
    return m_rect.Width;
}

int Meter::Height() {
    return m_rect.Height;
}

Gdiplus::Bitmap *Meter::Bitmap() {
    return m_meterBitmap;
}

void Meter::SetBitmap(Gdiplus::Bitmap *meterBitmap) {
    m_meterBitmap = meterBitmap;

    m_rect.Width = meterBitmap->GetWidth();
    m_rect.Height = meterBitmap->GetHeight();
}

bool Meter::Dirty() {
    /* Not dirty if the meter's value is unchanged */
    if (Value() == m_lastValue) {
        return false;
    }

    /* Not dirty if the change in value won't affect the meter graphically */
    if (CalcUnits() == m_lastUnits) {
        return false;
    }

    return true;
}

int Meter::CalcUnits() {
    return (int) (m_meterValue * (float) m_units + 0.5);
}