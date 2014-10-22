#pragma once

#include "MeterWnd\Meters\MeterTypes.h"
#include "MeterWnd\MeterWnd.h"
#include "MeterWnd\Meter.h"

class VolumeOSD {
public:
    VolumeOSD(HINSTANCE hInstance):
        _mWnd(hInstance, L"3RVX-VolumeOSD", L"3RVX-VolumeOSD") {
    };

    void MeterValues();
    void MeterValues(float value);

private:
    MeterWnd _mWnd;
};