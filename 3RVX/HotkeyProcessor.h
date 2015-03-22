#pragma once

class HotkeyInfo;

class HotkeyProcessor {
    virtual void ProcessHotkeys(HotkeyInfo &hki) = 0;
};