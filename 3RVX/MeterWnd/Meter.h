#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <string>

#include "../Logger.h"

class Meter {
public:
    Meter(std::wstring bitmapName, int x, int y, int units);
    Meter(int x, int y, int units);

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) = 0;
    virtual bool Dirty();

    virtual float Value() const;
    virtual void Value(float value);

    virtual int Units() const;

    virtual std::wstring ToString();

protected:
    int _units;
    Gdiplus::Bitmap *_bitmap;
    Gdiplus::Rect _rect;

    virtual int CalcUnits();
    void UpdateDrawnValues();

private:
    int _drawnUnits;
    float _value;
    float _drawnValue;
};