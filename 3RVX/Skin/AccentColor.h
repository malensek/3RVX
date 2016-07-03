#pragma once

#include <Windows.h>

class AccentColor {

public:
    AccentColor();
    UINT32 Color() const;
    void Color(UINT32 color);
    void Refresh();
    void UseSystemColor(bool enable = true);

private:
    UINT32 _color;
    bool _override = false;
    bool _useUndocumented = false;

    /// <summary>
    /// Retrieves system colorization parameters, which includes the raw
    /// colorization color used by DWM for titlebars and other accents. This 
    /// method returns a different color than the DwmGetColorizationColor()
    /// function, which doesn't retrieve the actual titlebar color. However,
    /// it also doesn't return the titlebar color for some themes in Windows 10
    /// or the automatic colorization based on wallpaper.
    /// <para>
    /// This method uses UNDOCUMENTED API calls and may cause horrific crashes
    /// in the future. You have been warned!
    /// </para>
    /// </summary>
    /// <returns>
    /// An integer representing the DWM accent color, or -1 if a failure occurs.
    /// </returns>
    INT64 ColorizationParamColor();
};