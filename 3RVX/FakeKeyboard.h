#pragma once

#include <Windows.h>

class FakeKeyboard {
public:
    static void SimulateKeypress(unsigned short vk) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = vk;
        input.ki.wScan = 0;
        input.ki.dwFlags = 0;
        input.ki.time = 0;
        input.ki.dwExtraInfo = GetMessageExtraInfo();

        /* key down: */
        SendInput(1, &input, sizeof(INPUT));

        /* key up: */
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
};