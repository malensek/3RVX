#pragma once

#include "../Animation.h"

class FadeOut : public Animation {
public:
    virtual bool Animate(MeterWnd &meterWnd);
    virtual void Reset(MeterWnd &meterWnd);
};