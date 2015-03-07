#pragma once

#include "..\Meter.h"
#include "..\MeterCallbackReceiver.h"

class CallbackMeter : public Meter {
public:
    CallbackMeter(int units, MeterCallbackReceiver &receiver) :
    Meter(0, 0, units),
    _receiver(receiver) {

    }

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
    virtual std::wstring ToString();

private:
    MeterCallbackReceiver &_receiver;
};