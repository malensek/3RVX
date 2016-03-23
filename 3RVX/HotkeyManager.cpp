// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "HotkeyManager.h"

#include "Logger.h"
#include "SyntheticKeyboard.h"

HotkeyManager *HotkeyManager::instance = NULL;

std::unordered_map<UINT, std::wstring> HotkeyManager::_vkStringMap = {
    { VK_SELECT, L"Select" },
    { VK_PRINT, L"Print" },
    { VK_EXECUTE, L"Execute" },
    { VK_SNAPSHOT, L"Print Screen" },
    { VK_HELP, L"Help" },
    { VK_SLEEP, L"Sleep" },
    { VK_BROWSER_BACK, L"Browser Back" },
    { VK_BROWSER_FORWARD, L"Browser Forward" },
    { VK_BROWSER_REFRESH, L"Browser Refresh" },
    { VK_BROWSER_STOP, L"Browser Stop" },
    { VK_BROWSER_SEARCH, L"Browser Search" },
    { VK_BROWSER_FAVORITES, L"Browser Favorites" },
    { VK_BROWSER_HOME, L"Browser Home" },
    { VK_VOLUME_MUTE, L"Mute" },
    { VK_VOLUME_DOWN, L"Volume Down" },
    { VK_VOLUME_UP, L"Volume Up" },
};

HotkeyManager *HotkeyManager::Instance() {
    return instance;
}

HotkeyManager *HotkeyManager::Instance(HWND notifyWnd) {
    if (instance == NULL) {
        instance = new HotkeyManager();
        instance->Hook();
        instance->_notifyWnd = notifyWnd;
    }

    return instance;
}

HotkeyManager::HotkeyManager() :
_fixWin(false) {

}

HotkeyManager::~HotkeyManager() {
    while (true) {
        auto i = _keyCombinations.begin();
        if (_keyCombinations.size() > 0) {
            Unregister(*i);
        } else {
            break;
        }
    }
    _hookCombinations.clear();

    Unhook();
}

void HotkeyManager::Shutdown() {
    delete instance;
    instance = NULL;
}

bool HotkeyManager::Hook() {
    _mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
        LowLevelMouseProc, NULL, NULL);

    _keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,
        LowLevelKeyboardProc, NULL, NULL);

    return _mouseHook && _keyHook;
}

bool HotkeyManager::Unhook() {
    BOOL unMouse = UnhookWindowsHookEx(_mouseHook);
    BOOL unKey = UnhookWindowsHookEx(_keyHook);
    return unMouse && unKey;
}

void HotkeyManager::Register(int keyCombination) {
    if (_keyCombinations.count(keyCombination) > 0) {
        CLOG(L"Hotkey combination [%d] already registered", keyCombination);
        return;
    } else {
        _keyCombinations.insert(keyCombination);
    }

    /* get VK_* value; */
    int vk = 0xFF & keyCombination;

    if ((keyCombination >> 20) > 0 /* uses a HKM_MOUSE_* flag */
        || vk == VK_LBUTTON
        || vk == VK_RBUTTON
        || vk == VK_MBUTTON) {

        /* mouse-based hotkeys; we are done */
        CLOG(L"Registered new mouse-based hotkey: %d", keyCombination);
        return;
    }

    /* keyboard-only hotkeys; use WinAPI */
    int mods = (0xF0000 & keyCombination) >> 16;
    if (!RegisterHotKey(_notifyWnd, keyCombination, mods, vk)) {
        CLOG(L"Failed to register hotkey [%d]\n"
            L"Mods: %d, VK: %d\n"
            L"Placing in hook list", keyCombination, mods, vk);
        _hookCombinations.insert(keyCombination);
        return;
    }

    CLOG(L"Registered new keyboard-based hotkey: %d", keyCombination);
}

bool HotkeyManager::Unregister(int keyCombination) {
    CLOG(L"Unregistering hotkey combination: %d", keyCombination);

    if (_keyCombinations.count(keyCombination) <= 0) {
        QCLOG(L"Hotkey combination [%d] was not previously registered",
            keyCombination);
        return false;
    }

    _keyCombinations.erase(keyCombination);
    if ((keyCombination >> 20) == 0) {
        /* This hotkey isn't mouse-based; unregister with Windows */
        if (!UnregisterHotKey(_notifyWnd, keyCombination)) {
            CLOG(L"Failed to unregister hotkey: %d", keyCombination);
            return false;
        }
    }
    return true;
}

LRESULT CALLBACK
HotkeyManager::KeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYUP) {
            KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT *) lParam;
            if ((kbInfo->vkCode == VK_LWIN || kbInfo->vkCode == VK_RWIN)
                && _fixWin) {
                /* WIN+Mouse combination used; we need to prevent the
                 * system from only seeing a WIN keypress (and usually
                 * popping up the start menu).  We simulate WIN+VK_NONAME. */
                SyntheticKeyboard::SimulateKeypress(VK_NONAME, false);
                _fixWin = false;
                return CallNextHookEx(NULL, nCode, wParam, lParam);
            }
        }

        if (_hookCombinations.size() <= 0) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT *) lParam;

            DWORD vk = kbInfo->vkCode;
            int mods = HotkeyManager::IsModifier(vk);

            if (mods) {
                _modifiers |= mods;
                return CallNextHookEx(NULL, nCode, wParam, lParam);
            } else {
                /* Is this an extended key? */
                int ext = (kbInfo->flags & 0x1) << EXT_OFFSET;
                int keys = _modifiers | ext | vk;
                if (_hookCombinations.count(keys) > 0) {
                    SendMessage(_notifyWnd, WM_HOTKEY,
                        keys, _modifiers >> MOD_OFFSET);
                    return (LRESULT) 1;
                }
            }
        }

        if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT *) lParam;
            int m = HotkeyManager::IsModifier(kbInfo->vkCode);
            if (m) {
                _modifiers ^= m;
            }
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
HotkeyManager::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        int mouseState = 0;
        MSLLHOOKSTRUCT *msInfo;

        switch (wParam) {

            case WM_LBUTTONDOWN:
                mouseState += VK_LBUTTON;
                break;

            case WM_MBUTTONDOWN:
                mouseState += VK_MBUTTON;
                break;

            case WM_RBUTTONDOWN:
                mouseState += VK_RBUTTON;
                break;

            case WM_XBUTTONDOWN: {
                msInfo = (MSLLHOOKSTRUCT *) lParam;

                int button = msInfo->mouseData >> 16 & 0xFFFF;
                if (button == 1)
                    mouseState += HKM_MOUSE_XB1;
                else if (button == 2)
                    mouseState += HKM_MOUSE_XB2;

                break;
            }

            case WM_MOUSEWHEEL: {
                /* Note: WM_MOUSEWHEEL on a hook is a little different, so we
                 * need to grab the wheel delta info from the lParam. */
                msInfo = (MSLLHOOKSTRUCT *) lParam;

                if ((int) msInfo->mouseData > 0) {
                    mouseState += HKM_MOUSE_WHUP;
                } else {
                    mouseState += HKM_MOUSE_WHDN;
                }

                break;
            }
        }

        if (mouseState > 0) {
            mouseState += Modifiers();

            if (_keyCombinations.count(mouseState) > 0) {
                PostMessage(_notifyWnd, WM_HOTKEY,
                    mouseState, mouseState & 0xF0000);

                if (mouseState & HKM_MOD_WIN) {
                    _fixWin = true; /* enable fix right before WIN goes up */
                }

                return 1; /* processed the message; eat it */
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK
HotkeyManager::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return HotkeyManager::instance->MouseProc(nCode, wParam, lParam);
}

LRESULT CALLBACK
HotkeyManager::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    return HotkeyManager::instance->KeyProc(nCode, wParam, lParam);
}

int HotkeyManager::IsModifier(int vk) {
    switch (vk) {
    case VK_MENU:
    case VK_LMENU:
    case VK_RMENU:
        return HKM_MOD_ALT;

    case VK_CONTROL:
    case VK_LCONTROL:
    case VK_RCONTROL:
        return HKM_MOD_CTRL;

    case VK_SHIFT:
    case VK_LSHIFT:
    case VK_RSHIFT:
        return HKM_MOD_SHF;

    case VK_LWIN:
    case VK_RWIN:
        return HKM_MOD_WIN;
    }

    return 0;
}

bool HotkeyManager::IsMouseKey(int vk) {
    if (vk < 0x07 && vk != VK_CANCEL) {
        return true;
    }

    if (vk & (0xF << MOUSE_OFFSET)) {
        /* Has wheel or xbutton flags */
        return true;
    }

    return false;
}

int HotkeyManager::Modifiers() {
    int mods = 0;
    mods += (GetKeyState(VK_MENU) & 0x8000) << 1;
    mods += (GetKeyState(VK_CONTROL) & 0x8000) << 2;
    mods += (GetKeyState(VK_SHIFT) & 0x8000) << 3;
    mods += (GetKeyState(VK_LWIN) & 0x8000) << 4;
    mods += (GetKeyState(VK_RWIN) & 0x8000) << 4;
    return mods;
}

int HotkeyManager::ModifiersAsync() {
    int mods = 0;
    mods += (GetAsyncKeyState(VK_MENU) & 0x8000) << 1;
    mods += (GetAsyncKeyState(VK_CONTROL) & 0x8000) << 2;
    mods += (GetAsyncKeyState(VK_SHIFT) & 0x8000) << 3;
    mods += (GetAsyncKeyState(VK_LWIN) & 0x8000) << 4;
    mods += (GetAsyncKeyState(VK_RWIN) & 0x8000) << 4;
    return mods;
}

std::wstring HotkeyManager::HotkeysToModString(int combination,
    std::wstring separator) {

    std::wstring str = L"";
    if (combination & HKM_MOD_ALT) {
        str += VKToString(VK_MENU) + separator;
    }
    if (combination & HKM_MOD_CTRL) {
        str += VKToString(VK_CONTROL) + separator;
    }
    if (combination & HKM_MOD_SHF) {
        str += VKToString(VK_SHIFT) + separator;
    }
    if (combination & HKM_MOD_WIN) {
        str += L"Win" + separator;
    }

    return str;
}

std::wstring HotkeyManager::HotkeysToString(int combination,
    std::wstring separator) {

    std::wstring mods = HotkeysToModString(combination, separator);
    int vk = combination & 0xFF;

    std::wstring str;
    if (IsMouseKey(vk)) {
        str = MouseString(combination);
    } else {
        bool ext = (combination & 0x100) > 0;
        str = VKToString(vk, ext);
    }

    return mods + str;
}

std::wstring HotkeyManager::MouseString(int combination) {
    int vk = combination & 0xFF;
    if (vk > 0) {
        switch (vk) {
        case VK_LBUTTON:
            return L"Mouse 1";
        case VK_RBUTTON:
            return L"Mouse 2";
        case VK_MBUTTON:
            return L"Mouse 3";
        }
    }

    int flags = combination & (0xF << MOUSE_OFFSET);
    if (flags == HKM_MOUSE_XB1) {
        return L"Mouse 4";
    } else if (flags == HKM_MOUSE_XB2) {
        return L"Mouse 5";
    } else if (flags == HKM_MOUSE_WHUP) {
        return L"Mouse Wheel Up";
    } else if (flags == HKM_MOUSE_WHDN) {
        return L"Mouse Wheel Down";
    }

    return L"";
}

std::wstring HotkeyManager::VKToString(unsigned int vk, bool extendedKey) {
    /* GetKeyNameText expects the following:
     * 16-23: scan code
     *    24: extended key flag
     *    25: 'do not care' bit (don't distinguish between L/R keys) */
    unsigned int scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    scanCode = scanCode << 16;

    if (vk == VK_RSHIFT) {
        /* For some reason, the right shift key ends up having its extended
         * key flag set and confuses GetKeyNameText. */
        extendedKey = false;
    }

    int extended = extendedKey ? 0x1 : 0x0;
    scanCode |= extended << 24;

    wchar_t buf[256] = {};
    GetKeyNameText(scanCode, buf, 256);
    return std::wstring(buf);
}