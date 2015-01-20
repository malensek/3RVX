#pragma once

#include <Windows.h>
#include <unordered_set>

#include "HotkeyManager.h"
#include "KeyReceiver.h"

class KeyGrabber {
public:
    static KeyGrabber *Instance();
    void Grab(KeyReceiver &receiver);
    void Stop();

private:
    HHOOK _keyHook;
    HHOOK _mouseHook;

    KeyReceiver *_receiver;

    bool Hook();
    bool Unhook();

    std::wstring VKToString(unsigned int vk, bool extendedKey = false);

    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyGrabber *instance;
};