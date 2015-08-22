#pragma once

#include "../3RVX/Window.h"

class UpdaterWindow : public Window {
public:
    UpdaterWindow();

private:
    HICON _icon;
    virtual LRESULT WndProc(HWND hWnd, UINT message,
        WPARAM wParam, LPARAM lParam);

public:
    static const int MSG_CREATEICON;
};