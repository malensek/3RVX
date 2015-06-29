#pragma once

#include <Windows.h>

#include <vector>

class FakeKeyboard {
public:
    static void SimulateKeypress(unsigned short vk);

private:
    static void PopulateInput(INPUT &in, unsigned short vk, bool up = false);

    /// <summary>
    /// Retrieves a vector of the keyboard modifiers that are currently being
    /// pressed.
    /// </summary>
    static std::vector<unsigned short> ModifiersDown();
};