// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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
    ~Meter();

    /// <summary>
    /// Draws the current meter state onto the specified buffer.
    /// </summary>
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) = 0;

    /// <summary>
    /// Reports whether or not the meter's state has changed since the last
    /// time it was drawn. If a meter is dirty, it should be redrawn.
    /// </summary>
    virtual bool Dirty();

    /// <summary>Retrieves the meter's current value (0 - 1.0)</summary>
    virtual float Value() const;

    /// <summary>Sets the meter value (0 - 1.0)</summary>
    virtual void Value(float value);

    /// <summary>
    /// Retrieves the maximum number of units shown by this meter.
    /// </summary>
    virtual int Units() const;
    /// <summary>
    /// Calculates the current number of units the meter should display.
    /// </summary>
    virtual int CalcUnits();

    int X() const;
    int Y() const;
    void X(int x);
    void Y(int y);
    int Width() const;
    int Height() const;

    /// <summary>
    /// Retrieves the current meter state as a string. This includes
    /// information such as the maximum number of units supported by
    /// the meter, current value, etc.
    /// </summary>
    virtual std::wstring ToString();

protected:
    int _units;
    Gdiplus::Bitmap *_bitmap;
    Gdiplus::Rect _rect;

    /// <summary>
    /// Updates state variables after a draw operation. This helps distinguish
    /// between a meter whose value has been changed, and a meter that has
    /// been drawn with a new value.
    /// </summary>
    void UpdateDrawnValues();

private:
    int _drawnUnits;
    float _value;
    float _drawnValue;
};