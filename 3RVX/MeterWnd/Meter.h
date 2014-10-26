#pragma once

#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
#include <string>

#include "../Logger.h"

class Meter {
public:
    Meter(std::wstring bitmapName, int x, int y, int units) :
        _value(0.0f),
        _lastValue(-1.0f),
        _units(units),
        _lastUnits(-1) {
        _rect.X = x;
        _rect.Y = y;

        Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(
            bitmapName.c_str(), false);
        CLOG(L"Loading meter bitmap: %s\nStatus: %d",
            bitmapName.c_str(), bmp->GetLastStatus());
        _bitmap = bmp;
    };

    enum TextAlignment { Left, Right, Center };

    virtual bool Dirty();
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) = 0;
    virtual int CalcUnits();

    virtual float Value();
    virtual void Value(float value);

    virtual Gdiplus::Rect Rect() const;
    virtual int X() const;
    virtual int Y() const;
    virtual int Width() const;
    virtual int Height() const;

    virtual int Units();

    virtual std::wstring ToString();

protected:
    Gdiplus::Bitmap *_bitmap;
    float _value;
    float _lastValue;
    int _units;
    int _lastUnits;
    Gdiplus::Rect _rect;
};