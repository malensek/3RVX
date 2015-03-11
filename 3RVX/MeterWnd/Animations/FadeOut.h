#pragma once

#include <cmath>

#include "../Animation.h"

class FadeOut : public Animation {
public:
    FadeOut(int speed);

    virtual bool Animate(MeterWnd *meterWnd);
    virtual void Reset(MeterWnd *meterWnd);

    virtual int UpdateInterval();

private:
    int _interval;
    int _step;
};