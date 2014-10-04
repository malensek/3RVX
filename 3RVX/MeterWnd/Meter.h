#pragma once
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

class Meter {
public:
    Meter() : m_meterBitmap(NULL),
        m_units(-1),
        m_lastUnits(-1),
        m_meterValue(-1),
        m_lastValue(-1),
        m_reversed(false) {
    };

    // Init() is called AFTER all params are set up!  For example, the buffer
    // bitmap, units, etc. should be set before Init() is called.
    virtual void Init();
    virtual bool Dirty();
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) = 0;
    virtual int CalcUnits();

    virtual float Value();
    virtual void SetValue(float value);

    virtual int X();
    virtual int Y();
    virtual int Width();
    virtual int Height();
    virtual Gdiplus::Rect Rect();
    virtual void SetLocation(int x, int y);
    virtual void SetSize(int width, int height);

    virtual bool Reversed();
    virtual void SetReverse(bool reversed);

    virtual int Units();
    virtual void SetUnits(int units);

    virtual Gdiplus::Bitmap *Bitmap();
    virtual void SetBitmap(Gdiplus::Bitmap *meterBitmap);

protected:
    Gdiplus::Bitmap *m_meterBitmap;
    float m_meterValue;
    float m_lastValue;
    Gdiplus::Rect m_rect;
    bool m_reversed;
    int m_units;
    int m_lastUnits;
};