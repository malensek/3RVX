#pragma once

#include "MeterWnd.h"

class Animation {
public:
    Animation(MeterWnd &meterWnd) :
    _meterWnd(meterWnd) {

    }

    virtual bool Animate() = 0;
    virtual void Reset() = 0;

protected:
    MeterWnd &_meterWnd;
};