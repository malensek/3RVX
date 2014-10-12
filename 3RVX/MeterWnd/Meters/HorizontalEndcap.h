#pragma once

#include "../Meter.h"

class HorizontalEndcap : public Meter {
public:
    HorizontalEndcap() : m_lMargin(-1),
        m_rMargin(-1),
        m_unitWidth(-1) {
    };

    virtual void Init();
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    virtual void SetBitmap(Gdiplus::Bitmap *meterBitmap);
    virtual void SetValue(float value);

protected:
    int m_lMargin;
    int m_rMargin;
    int m_unitWidth;

    Gdiplus::Rect m_lMarginRect;
    Gdiplus::Rect m_rMarginRect;
};