#include "HorizontalEndcap.h"

void HorizontalEndcap::Init() {
    m_lMarginRect.X = X();
    m_lMarginRect.Y = Y();
    m_lMarginRect.Width = m_lMargin;
    m_lMarginRect.Height = Height();

    m_rMarginRect.Y = Y();
    m_rMarginRect.Width = m_meterBitmap->GetWidth() - m_rMargin + 1;
    m_rMarginRect.Height = Height();

    SetValue(m_meterValue);
}

void HorizontalEndcap::SetValue(float value) {
    Meter::SetValue(value);
    m_rect.Width = m_lMargin + m_unitWidth * CalcUnits() + m_rMargin;
}

void
HorizontalEndcap::Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics) {
    int units = CalcUnits();

    Gdiplus::Rect destRect(X(), Y(), m_lMarginRect.Width, Height());

    /* draw the left endcap */
    graphics->DrawImage(m_meterBitmap, destRect,
        0, 0, m_lMargin, Height(), Gdiplus::UnitPixel);

    /* draw the middle of the bar */
    destRect.X = destRect.X + m_lMargin;
    destRect.Width = m_unitWidth * units;

    Gdiplus::TextureBrush tB(m_meterBitmap, Gdiplus::WrapModeTile,
        m_lMargin + 1, 0, m_rMargin - m_lMargin - 1, Height());

    /* translate the texture brush into position.
     * note, 1st param = how much X stretch, 4th = how much Y stretch */
    Gdiplus::Matrix mat(1, 0, 0, 1, (float) destRect.X, (float) destRect.Y);
    tB.SetTransform(&mat);

    graphics->FillRectangle(&tB, destRect);

    /* draw the right endcap */
    destRect.X = destRect.X + destRect.Width;
    destRect.Width = m_rMarginRect.Width;
    graphics->DrawImage(m_meterBitmap, destRect,
        m_rMargin + 1, 0, m_rMarginRect.Width, Height(), Gdiplus::UnitPixel);

    m_lastValue = Value();
    m_lastUnits = units;
}

void HorizontalEndcap::SetBitmap(Gdiplus::Bitmap *meterBitmap) {
    using namespace Gdiplus;

    Color currentPixel;
    ARGB searchColor = 0xFFFF00FF; //magic pink (FF,00,FF)
    unsigned int width = meterBitmap->GetWidth();

    for (unsigned int x = 0; x < width; ++x) {
        meterBitmap->GetPixel(x, 0, &currentPixel);
        if (currentPixel.GetValue() == searchColor) {
            if (m_lMargin < 0) {
                m_lMargin = x;
            } else {
                m_rMargin = x;
                break;
            }
        }
    }

    /* 1 extra px for L-margin line */
    m_unitWidth = m_rMargin - m_lMargin - 1;
    m_rect.Height = meterBitmap->GetHeight();

    m_meterBitmap = meterBitmap;
}