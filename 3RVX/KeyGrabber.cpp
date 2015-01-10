#include "KeyGrabber.h"

#include "Logger.h"

KeyGrabber *KeyGrabber::instance = NULL;

KeyGrabber *KeyGrabber::Instance() {
    if (instance == NULL) {
        instance = new KeyGrabber();
    }

    return instance;
}

bool KeyGrabber::Hook() {
    _mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
        LowLevelMouseProc, NULL, NULL);

    _keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,
        LowLevelKeyboardProc, NULL, NULL);

    return _mouseHook && _keyHook;
}

bool KeyGrabber::Unhook() {
    BOOL unMouse = UnhookWindowsHookEx(_mouseHook);
    BOOL unKey = UnhookWindowsHookEx(_keyHook);
    return unMouse && unKey;
}

int KeyGrabber::Modifiers() {
    int mods = 0;
    mods += (GetKeyState(VK_MENU) & 0x8000) << 1;
    mods += (GetKeyState(VK_CONTROL) & 0x8000) << 2;
    mods += (GetKeyState(VK_SHIFT) & 0x8000) << 3;
    mods += (GetKeyState(VK_LWIN) & 0x8000) << 4;
    mods += (GetKeyState(VK_RWIN) & 0x8000) << 4;
    return mods;
}

void KeyGrabber::Grab(HWND hwnd) {
    _updateHwnd = hwnd;
    Hook();
}

LRESULT CALLBACK
KeyGrabber::KeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT*) lParam;

        LONG l = kbInfo->scanCode << 16;
        LONG ext = (kbInfo->flags & 0x1) << 24;
        l = l | ext;
        l = l | (0x1 << 25);
        if (kbInfo->vkCode == VK_RSHIFT) {
            l = 0x2360000;
        }
        wchar_t buf[256] = {};
        GetKeyNameText(l, buf, 256);
        CLOG(L"key: %s", buf);
        CLOG(L"hex: %x", l);
        CLOG(L"VK: %x", kbInfo->vkCode);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
KeyGrabber::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
KeyGrabber::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return KeyGrabber::instance->MouseProc(nCode, wParam, lParam);
}

LRESULT CALLBACK
KeyGrabber::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return KeyGrabber::instance->KeyProc(nCode, wParam, lParam);
}