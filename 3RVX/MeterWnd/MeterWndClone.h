#pragma once

#include "MeterWnd.h"

class MeterWndClone {
public:
    MeterWndClone(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL);
    ~MeterWndClone();

    void Show();
    void Hide();

    void Update(Gdiplus::Bitmap *composite);
    void Transparency(byte transparency);
    void GlassMask(Gdiplus::Bitmap *mask);

    MeterWnd &MeterWindow();

private:
    MeterWnd _mWnd;
};