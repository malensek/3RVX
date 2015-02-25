#include "CallbackMeter.h"

void CallbackMeter::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();
    _receiver.MeterChangeCallback(units);
    UpdateDrawnValues();
}