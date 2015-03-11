#pragma once

class MeterWnd;

class Animation {
public:
    virtual bool Animate(MeterWnd *meterWnd) = 0;
    virtual void Reset(MeterWnd *meterWnd) = 0;
};