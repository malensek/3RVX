#pragma once

#include <Windows.h>

class FakeKeyboard {
public:
    static void SimulateKeypress(unsigned short vk) {
        unsigned int scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);

        INPUT input[2];

        input[0] = { 0 };
        input[0].type = INPUT_KEYBOARD;
        input[0].ki.wVk = vk;
        input[0].ki.wScan = scan;
        input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
        input[0].ki.time = 0;
        input[0].ki.dwExtraInfo = GetMessageExtraInfo();

        input[1] = { 0 };
        input[1].type = INPUT_KEYBOARD;
        input[1].ki.wVk = vk;
        input[1].ki.wScan = scan;
        input[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        input[1].ki.time = 0;
        input[1].ki.dwExtraInfo = GetMessageExtraInfo();

        UINT result = SendInput(2, input, sizeof(INPUT));
    }
};