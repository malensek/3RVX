#pragma once

#include "MeterWnd\MeterWnd.h"

class SliderWnd : public MeterWnd {
public:
    SliderWnd(LPCWSTR className, LPCWSTR title, HINSTANCE hInstance = NULL);

    virtual void Show();

private:

    void PositionWindow();

    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};