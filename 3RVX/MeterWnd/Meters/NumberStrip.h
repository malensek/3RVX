#ifndef NUMBERSTRIP_H
#define NUMBERSTRIP_H

#include "../../StdAfx.h"
#include "../Meter.h"
#include <string>

#define NUMBERSTRIP_ALIGN_LEFT   0x01
#define NUMBERSTRIP_ALIGN_RIGHT  0x02
#define NUMBERSTRIP_ALIGN_CENTER 0x03

class NumberStrip : public Meter {
public:
    virtual void Init();

    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);

    virtual void SetAlignment(const wchar_t *alignment);
    virtual void SetColumns(byte cols);

private:
    byte m_align;
    byte m_cols;
    int  m_charHeight;
};

#endif