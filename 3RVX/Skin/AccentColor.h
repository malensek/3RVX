#pragma once

#include <Windows.h>

class AccentColor {

public:
    /// <summary>Retrieves the instance of the AccentColor singleton.</summary>
    static AccentColor *Instance();

    /// <summary>Retrieves the accent color.</summary>
    UINT32 Color() const;

    /// <summary>
    /// Sets the accent color manually. This will override system accent colors.
    /// </summary>
    void Color(UINT32 color);

    /// <summary>
    /// Refreshes the system accent color. If an accent was specified manually,
    /// calling this method will produce no result. This is helpful when the
    /// system color scheme changes (either automatically from a wallpaper
    /// slideshow or from a user preference change).
    /// </summary>
    void Refresh();

    /// <summary>
    /// Determines whether or not to use the system accent color.
    /// </summary>
    /// <param name="enable">
    /// If set to true, the system accent color will be queried and cached for
    /// future use. Otherwise, a manually-specified color (defined with the
    /// Color(UINT32) method) will be used.
    /// </param>
    void UseSystemColor(bool enable = true);

private:
    AccentColor();
    ~AccentColor();
    static AccentColor *instance;

    /// <summary>Cached accent color</summary>
    UINT32 _color;

    /// <summary>Whether or not to override the system color scheme.</summary>
    bool _override = false;

    /// <summary>
    /// Whether or not to use undocumented DWM APIs to retrieve the accurate
    /// theme colorization color.
    /// </summary>
    bool _useUndocumented = false;

    /// <summary>
    /// If undocumented DWM functions are used, the module handle is cached
    /// here for future use.
    /// </summary>
    HMODULE _dwmLib;

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