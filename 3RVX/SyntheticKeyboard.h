// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include <vector>

/// <summary>
/// Creates synthetic keyboard events, like activating a media key or virtual
/// keycode.
/// </summary>
class SyntheticKeyboard {
public:
    /// <summary>
    /// Simulates a keypress (key down, key up) of the given VK code. If 
    /// toggleModifiers is true, then any modifier keys that are down when this
    /// method is called are synthetically raised, the keypress is made, and
    /// then then modifiers are placed back in their original state.
    /// </summary>
    /// <param name="vk">The virtual key code to 'press.'</param>
    /// <param name="handleModifiers">
    /// Whether or not modifier keys should be raised and then re-lowered while
    /// synthesizing the keypress.
    /// </param>
    static void SimulateKeypress(
        unsigned short vk, bool handleModifiers = true);

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