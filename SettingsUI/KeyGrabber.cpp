#include "KeyGrabber.h"

#include <string>

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

void KeyGrabber::Grab() {
    Hook();
}

void KeyGrabber::SetHwnd(HWND updateHwnd) {
    _hWnd = updateHwnd;
}

int KeyGrabber::KeyCombination() {
    return _keyCombination;
}

LRESULT CALLBACK
KeyGrabber::KeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT *) lParam;

        DWORD vk = kbInfo->vkCode;
        int mods = HotkeyManager::ModifiersAsync();
        if (HotkeyManager::IsModifier(kbInfo->vkCode)
            || (vk == VK_ESCAPE && mods == 0)) {
            /* Ignore modifier keys (since we determine their state manually
             * later) and pass Esc through to let the user cancel the
             * operation. */
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        /* GetKeyNameText expects the following:
         * 16-23: scan code
         *    24: extended key flag
         *    25: 'do not care' bit (don't distinguish between L/R keys) */
        BOOL dontCare = TRUE;
        LONG newlParam;
        newlParam = (kbInfo->scanCode << 16);
        newlParam |= (kbInfo->flags & 0x1) << 24;
        newlParam |= dontCare << 25;
        if (kbInfo->vkCode == VK_RSHIFT) {
            /* For some reason, the right shift key ends up having its extended
             * key flag set and then prints the wrong thing. This doesn't matter
             * here, but we'll fix it in case we need this info later. */
            newlParam ^= 0x1000000;
        }

        wchar_t buf[256] = {};
        GetKeyNameText(newlParam, buf, 256);
        int mods = HotkeyManager::ModifiersAsync();
        std::wstring modStr = HotkeyManager::HotkeysToModString(mods) + buf;

        if (vk == VK_ESCAPE && mods == 0) {
            /* Pass escape through to let the user cancel the operation */
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        //SetWindowText(_updateHwnd, modStr.c_str());
        _keyCombination = mods + vk;
        PostMessage(_hWnd, WM_CLOSE, NULL, NULL);
        Unhook();

        /* Prevent other applications from receiving this event */
        return (LRESULT) 1;
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
KeyGrabber::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    unsigned int keyCombo = 0;

    switch (wParam) {
    case WM_LBUTTONDOWN:
        keyCombo = VK_LBUTTON;
        break;

    case WM_RBUTTONDOWN:
        keyCombo = VK_RBUTTON;
        break;

    case WM_MBUTTONDOWN:
        keyCombo = VK_MBUTTON;
        break;

    case WM_XBUTTONDOWN: {
        MSLLHOOKSTRUCT *msInfo = (MSLLHOOKSTRUCT *) lParam;
        int x = HIWORD(msInfo->mouseData);
        if (x == 1) {
            keyCombo = HKM_MOUSE_XB1;
        } else if (x == 2) {
            keyCombo = HKM_MOUSE_XB2;
        }
        break;
    }

    case WM_MOUSEWHEEL:
        short zDelta = GET_WHEEL_DELTA_WPARAM(lParam);
        if (zDelta > 0) {
            keyCombo = HKM_MOUSE_WHUP;
        } else if (zDelta < 0) {
            keyCombo = HKM_MOUSE_WHDN;
        }

        break;
    }

    if (keyCombo > 0) {
        int mods = HotkeyManager::ModifiersAsync();
        if (mods == 0 && keyCombo == VK_LBUTTON) {
            /* We require at least one modifier key with the left button. */
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        _keyCombination = mods + keyCombo;
        PostMessage(_hWnd, WM_CLOSE, NULL, NULL);
        Unhook();
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