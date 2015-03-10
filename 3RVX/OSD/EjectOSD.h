#pragma once

#include <Windows.h>

#include "OSD.h"

class EjectOSD : public OSD {
public:
    EjectOSD();
    ~EjectOSD();

    virtual void Hide();
    virtual void ProcessHotkeys(HotkeyInfo &hki);

private:
    MeterWnd _mWnd;

    void EjectDrive(std::wstring driveLetter);
    virtual void UpdateWindowPositions(std::vector<Monitor> monitors);

    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);
};