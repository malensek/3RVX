#pragma once

#include "../Animation.h"

class FadeOut : public Animation {
public:
    FadeOut(MeterWnd &meterWnd) :
        Animation(meterWnd) { }

    virtual bool Animate();
    virtual void Reset();
};