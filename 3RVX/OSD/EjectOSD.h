#pragma once

#include <Windows.h>

#include "OSD.h"

class EjectOSD : public OSD {
public:
    EjectOSD();
    ~EjectOSD();

    virtual void Hide();

private:
    MeterWnd _mWnd;

    virtual void UpdateWindowPositions(std::vector<HMONITOR> monitors);

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};