// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

//
//  Key Storage Format (24 bits):
//  
//  0111 1111 0000 0001 1111 1111
//  ---- ---- --------- ---------
//   |    |           |     |
//   |    |           |     \--- MS Virtual Keys (VK_*)
//   |    |           |
//   |    |           \--- Extended key bit (used for converting HKs to strings)
//   |    |
//   |    \--- bitfield: [win, shift, control, alt]
//   |
//   \--- xbutton2, xbutton1, wheelup, wheeldown
//

#pragma once

#include <Windows.h>
#include <unordered_set>

#define EXT_OFFSET 8
#define MOD_OFFSET 16
#define MOUSE_OFFSET 20

#define HKM_MOD_ALT (MOD_ALT << MOD_OFFSET)
#define HKM_MOD_CTRL (MOD_CONTROL << MOD_OFFSET)
#define HKM_MOD_SHF (MOD_SHIFT << MOD_OFFSET)
#define HKM_MOD_WIN (MOD_WIN << MOD_OFFSET)

#define HKM_MOUSE_WHUP (0x1 << MOUSE_OFFSET)
#define HKM_MOUSE_WHDN (0x2 << MOUSE_OFFSET)
#define HKM_MOUSE_XB1 (0x3 << MOUSE_OFFSET)
#define HKM_MOUSE_XB2 (0x4 << MOUSE_OFFSET)

class HotkeyManager {  
public:
    static HotkeyManager *Instance();
    static HotkeyManager *Instance(HWND notifyWnd);

    void Register(int keyCombination);
    bool Unregister(int keyCombination);

    void Shutdown();

public:
    static int IsModifier(int vk);
    static bool IsMouseKey(int vk);
    static int Modifiers();
    static int ModifiersAsync();
    static std::wstring HotkeysToModString(int combination,
        std::wstring separator = L" + ");
    static std::wstring HotkeysToString(int combination,
        std::wstring separator = L" + ");
    static std::wstring MouseString(int combination);
    static std::wstring VKToString(unsigned int vk, bool extendedKey = false);

private:
    HotkeyManager();
    ~HotkeyManager();

    HWND _notifyWnd;
    int _fixWin;
    std::unordered_set<int> _keyCombinations;
    std::unordered_set<int> _hookCombinations;
    int _modifiers;

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

    static HotkeyManager *instance;
};