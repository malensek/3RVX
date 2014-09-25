#include "HotkeyManager.h"

HotkeyManager* HotkeyManager::instance = NULL;

bool HotkeyManager::Hook()
{
    m_mouseHook = SetWindowsHookEx(WH_MOUSE_LL,
        LowLevelMouseProc, NULL, NULL);

    m_keyHook = SetWindowsHookEx(WH_KEYBOARD_LL,
        LowLevelKeyboardProc, NULL, NULL);

    return m_mouseHook && m_keyHook;
}

bool HotkeyManager::Unhook()
{
    return UnhookWindowsHookEx(m_mouseHook) && UnhookWindowsHookEx(m_keyHook);
}

HotkeyManager* HotkeyManager::Instance()
{
    if(instance == NULL)
    {
        instance = new HotkeyManager;
        instance->Hook();
    }

    return instance;
}

int HotkeyManager::Register(HWND parentWnd, int keyCombination)
{
    HotkeyInfo *hkInfo = new HotkeyInfo;
    hkInfo->hotkeyId = m_numKeys++;
    hkInfo->keys = keyCombination;
    hkInfo->parentWnd = parentWnd;

    if(m_keyMap.find(keyCombination) == m_keyMap.end())
        m_keyMap[keyCombination] = hkInfo;
    else
        return NULL;

    // get VK_* value; includes unused bits
    int vk = 0xFFFF & keyCombination;
    

    if((keyCombination >> 20) > 0 //uses a HKM_MOUSE_* flag
        || vk == VK_LBUTTON 
        || vk == VK_RBUTTON 
        || vk == VK_MBUTTON)
    {
        //mouse-based hotkeys; we are done
        return hkInfo->hotkeyId;
    }

    //keyboard-only hotkeys; use WinAPI
    int mods = (0xF0000 & keyCombination) >> 16;
    if(!RegisterHotKey(parentWnd, hkInfo->hotkeyId, mods, vk))
        return NULL;

    return hkInfo->hotkeyId;
}

int HotkeyManager::CalcModifiers()
{
    int mods = 0;
    mods += (GetKeyState(VK_MENU)    & 0x8000) << 1;
    mods += (GetKeyState(VK_CONTROL) & 0x8000) << 2;
    mods += (GetKeyState(VK_SHIFT)   & 0x8000) << 3;
    mods += (GetKeyState(VK_LWIN)    & 0x8000) << 4;
    mods += (GetKeyState(VK_RWIN)    & 0x8000) << 4;
    return mods;
}

LRESULT CALLBACK 
HotkeyManager::KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        if(wParam == WM_KEYUP)
        {
            KBDLLHOOKSTRUCT *kbInfo = (KBDLLHOOKSTRUCT*)lParam;
            if((kbInfo->vkCode == VK_LWIN || kbInfo->vkCode == VK_RWIN)
                && m_fixWin)
            {
                // WIN+Mouse combination used; we need to prevent the
                // system from only seeing a WIN keypress (and usually
                // popping up the start menu).  We simulate WIN+VK_NONAME.

                INPUT input = {0};
                input.type = INPUT_KEYBOARD;

                input.ki.wVk = VK_NONAME;
                input.ki.wScan = 0;
                input.ki.dwFlags = 0;
                input.ki.time = 1;
                input.ki.dwExtraInfo = GetMessageExtraInfo();

                SendInput(1, &input, sizeof(INPUT)); // key down

                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT)); // key up

                m_fixWin = false;
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK 
HotkeyManager::MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(nCode >= 0)
    {
        int mouseState = 0;
        MSLLHOOKSTRUCT *msInfo;

        switch(wParam)
        {
        case WM_LBUTTONDOWN:
            mouseState += VK_LBUTTON;
            break;
        case WM_MBUTTONDOWN:
            mouseState += VK_MBUTTON;
            break;
        case WM_RBUTTONDOWN:
            mouseState += VK_RBUTTON;
            break;

        case WM_XBUTTONDOWN:
            {
                msInfo = (MSLLHOOKSTRUCT*)lParam;

                int button = msInfo->mouseData >> 16 & 0xFFFF;
                if(button == 1)
                    mouseState += HKM_MOUSE_XBUTTON_1;
                else if(button == 2)
                    mouseState += HKM_MOUSE_XBUTTON_2;

                break;
            }

        case WM_MOUSEWHEEL:
                msInfo = (MSLLHOOKSTRUCT*)lParam;

                if((int)msInfo->mouseData > 0)
                    mouseState += HKM_MOUSE_WHEEL_UP;
                else
                    mouseState += HKM_MOUSE_WHEEL_DOWN;

                break;
        }

        if(mouseState > 0) 
        {
            mouseState += CalcModifiers();

            HotkeyInfo *hkInfo = m_keyMap[mouseState];
            if(hkInfo != NULL)
            {
                PostMessage(hkInfo->parentWnd, WM_HOTKEY, 
                    hkInfo->hotkeyId, hkInfo->keys & 0xF0000);

                if(mouseState & HKM_MOD_WIN)
                {
                    m_fixWin = true; // fix right before WIN key goes up
                }

                return 1; //processed the message; eat it
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//
// Pass messages back to the hkm object
//
LRESULT CALLBACK 
HotkeyManager::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return HotkeyManager::Instance()->MouseProc(nCode, wParam, lParam);
}

LRESULT CALLBACK 
HotkeyManager::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    return HotkeyManager::Instance()->KeyProc(nCode, wParam, lParam);
}