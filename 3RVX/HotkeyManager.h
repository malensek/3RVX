//
//  Key Storage Format (24 bits):
//  
//  0111 1111 0000 0000 1111 1111
//  ---- ---- --------- ---------
//   |    |       |         |
//   |    |       |         \--- MS Virtual Keys (VK_*)
//   |    |       |
//   |    |       \--- not used
//   |    |
//   |    \--- bitfield: [win, shift, control, alt]
//   |
//   \--- xbutton2, xbutton1, wheelup, wheeldown
//

#pragma once

#include <Windows.h>
#include <unordered_set>

#define HKM_MOD_ALT (MOD_ALT << 16)
#define HKM_MOD_CTRL (MOD_CONTROL << 16)
#define HKM_MOD_SHF (MOD_SHIFT << 16)
#define HKM_MOD_WIN (MOD_WIN << 16)

#define HKM_MOUSE_WHUP 0x100000
#define HKM_MOUSE_WHDN 0x200000
#define HKM_MOUSE_XB1 0x300000
#define HKM_MOUSE_XB2 0x400000

class HotkeyManager {  
public:
    static HotkeyManager *Instance();
    static HotkeyManager *Instance(HWND notifyWnd);

    void Register(int keyCombination);
    bool Unregister(int keyCombination);

    void Shutdown();

private:
    HotkeyManager();
    ~HotkeyManager();

    HWND _notifyWnd;
    int _fixWin;
    std::unordered_set<int> _keyCombinations;

    HHOOK _keyHook;
    HHOOK _mouseHook;

    bool Hook();
    bool Unhook();

    static int Modifiers();
    static std::wstring VKToString(unsigned int vk, bool extendedKey = false);

    LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

    LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK 
        LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    static HotkeyManager *instance;
};