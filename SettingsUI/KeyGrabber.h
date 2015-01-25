#pragma once

#include <Windows.h>
#include "HotkeyManager.h"

class KeyGrabber {
public:
    static KeyGrabber *Instance();

    void Grab();
    int KeyCombination();
    void SetHwnd(HWND updateHwnd);

private:
    HWND _hWnd;
    int _keyCombination;
    HHOOK _keyHook;
    HHOOK _mouseHook;

    bool Hook();
    bool Unhook();

    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyGrabber *instance;
};