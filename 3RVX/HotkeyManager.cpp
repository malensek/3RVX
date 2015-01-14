#include "HotkeyManager.h"
#include "Logger.h"

HotkeyManager *HotkeyManager::instance = NULL;

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

    /* get VK_* value; includes unused bits */
    int vk = 0xFFFF & keyCombination;

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
            L"Mods: %d, VK: %d", keyCombination, mods, vk);
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

int HotkeyManager::Modifiers() {
    int mods = 0;
    mods += (GetKeyState(VK_MENU) & 0x8000) << 1;
    mods += (GetKeyState(VK_CONTROL) & 0x8000) << 2;
    mods += (GetKeyState(VK_SHIFT) & 0x8000) << 3;
    mods += (GetKeyState(VK_LWIN) & 0x8000) << 4;
    mods += (GetKeyState(VK_RWIN) & 0x8000) << 4;
    return mods;
}

std::wstring HotkeyManager::VKToString(unsigned int vk, bool extendedKey) {
    int extended = extendedKey ? 0x1 : 0x0;

    unsigned int scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    scanCode = scanCode << 16;
    scanCode |= extended << 24;
    wchar_t buf[256] = {};
    GetKeyNameText(scanCode, buf, 256);
    return std::wstring(buf);
}
LRESULT CALLBACK
HotkeyManager::KeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYUP) {
            KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT*) lParam;
            if ((kbInfo->vkCode == VK_LWIN || kbInfo->vkCode == VK_RWIN)
                && _fixWin) {
                /* WIN+Mouse combination used; we need to prevent the
                 * system from only seeing a WIN keypress (and usually
                 * popping up the start menu).  We simulate WIN+VK_NONAME. */

                INPUT input = { 0 };
                input.type = INPUT_KEYBOARD;

                input.ki.wVk = VK_NONAME;
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 1;
                input.ki.dwExtraInfo = GetMessageExtraInfo();

                /* key down: */
                SendInput(1, &input, sizeof(INPUT));

                input.ki.dwFlags = KEYEVENTF_KEYUP;
                /* key up: */
                SendInput(1, &input, sizeof(INPUT));

                _fixWin = false;
            }
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
                msInfo = (MSLLHOOKSTRUCT*) lParam;

                int button = msInfo->mouseData >> 16 & 0xFFFF;
                if (button == 1)
                    mouseState += HKM_MOUSE_XB1;
                else if (button == 2)
                    mouseState += HKM_MOUSE_XB2;

                break;
            }

            case WM_MOUSEWHEEL: {
                msInfo = (MSLLHOOKSTRUCT*) lParam;

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