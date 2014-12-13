#pragma once

#include "MeterWnd\MeterWnd.h"

class SliderWnd : public MeterWnd {
public:
    SliderWnd(HINSTANCE hInstance, LPCWSTR className, LPCWSTR title);

    virtual void Show();

private:
    virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
};