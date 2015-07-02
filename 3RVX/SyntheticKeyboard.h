#pragma once

#include <Windows.h>

#include <vector>

class SyntheticKeyboard {
public:
    /// <summary>
    /// Simulates a keypress (key down, key up) of the given VK code. Any
    /// modifier keys that are down when this method is called are synthetically
    /// raised, the keypress is made, and then then modifiers are placed back
    /// in their original state.
    /// </summary>
    /// <param name="vk">The virtual key code to 'press.'</param>
    static void SimulateKeypress(unsigned short vk);

private:
    /// <summary>
    /// Populates an INPUT struct with a VK code, scan code, appropriate flags,
    /// etc for simulating keyboard events.
    /// </summary>
    static void PopulateInput(INPUT &in, unsigned short vk, bool up = false);

    /// <summary>
    /// Retrieves a vector of the keyboard modifiers that are currently being
    /// pressed.
    /// </summary>
    static std::vector<unsigned short> ModifiersDown();
};