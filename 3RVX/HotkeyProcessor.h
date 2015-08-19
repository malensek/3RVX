// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

class HotkeyInfo;

/// <summary>
/// Interface for classes that are able to handle and process hotkey events.
/// </summary>
class HotkeyProcessor {
    virtual void ProcessHotkeys(HotkeyInfo &hki) = 0;
};