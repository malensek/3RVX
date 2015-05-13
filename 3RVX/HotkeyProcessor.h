#pragma once

class HotkeyInfo;

/// <summary>
/// Interface for classes that are able to handle and process hotkey events.
/// </summary>
class HotkeyProcessor {
    virtual void ProcessHotkeys(HotkeyInfo &hki) = 0;
};