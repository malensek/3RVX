#pragma once

#include <Windows.h>
#include <unordered_set>

#include "HotkeyManager.h"

class KeyGrabber {
public:
    static KeyGrabber *Instance();
    void Grab(HWND hwnd);

private:
    HHOOK _keyHook;
    HHOOK _mouseHook;
    HWND _updateHwnd;

    bool Hook();
    bool Unhook();
    int Modifiers();

    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyGrabber *instance;
};