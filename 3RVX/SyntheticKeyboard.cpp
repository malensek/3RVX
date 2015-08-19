// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#include "SyntheticKeyboard.h"

#include "Logger.h"

void SyntheticKeyboard::SimulateKeypress(
        unsigned short vk, bool handleModifiers) {

    std::vector<unsigned short> mods;
    if (handleModifiers) {
        mods = ModifiersDown();
    }

    unsigned int inputs = mods.size() * 2 + 2;
    unsigned int currentInput = 0;
    INPUT *input = new INPUT[inputs];

    /* First, raise all modifier keys */
    for (; currentInput < mods.size(); ++currentInput) {
        PopulateInput(input[currentInput], mods[currentInput], true);
    }

    /* Simulate the keypress */
    PopulateInput(input[currentInput++], vk, false);
    PopulateInput(input[currentInput++], vk, true);

    /* Put modifier keys back to their original state */
    for (int i = 0; currentInput < inputs; ++i, ++currentInput) {
        PopulateInput(input[currentInput], mods[i], false);
    }

    UINT result = SendInput(inputs, input, sizeof(INPUT));
    CLOG(L"Simulating keypress: %x; %d keyboard inputs generated.",
        vk, result);
    delete[] input;
}

void SyntheticKeyboard::PopulateInput(INPUT &in, unsigned short vk, bool up) {
    in = { 0 };
    in.type = INPUT_KEYBOARD;
    in.ki.wVk = vk;
    in.ki.wScan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    in.ki.dwFlags = 0;
    in.ki.time = 0;
    in.ki.dwExtraInfo = GetMessageExtraInfo();

    if (up) {
        in.ki.dwFlags |= KEYEVENTF_KEYUP;
    }
}

std::vector<unsigned short> SyntheticKeyboard::ModifiersDown() {
    std::vector<unsigned short> mods;

    unsigned short modList[] = {
        VK_LSHIFT, VK_RSHIFT,
        VK_LCONTROL, VK_RCONTROL,
        VK_LMENU, VK_RMENU,
        VK_LWIN, VK_RWIN
    };

    for (unsigned short vk : modList) {
        if (GetAsyncKeyState(vk) & 0x8000) {
            mods.push_back(vk);
        }
    }

    return mods;
}