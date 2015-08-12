#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>

#include <list>
#include <vector>

struct OSDComponent;
struct SkinComponent;
struct SliderComponent;

class Skin {
public:
    /// <summary>Loads and instantiates the Volume OSD.</summary>
    virtual OSDComponent *VolumeOSD() = 0;

    /// <summary>Retrieves the iconset used for the volume icon.</summary>
    virtual std::vector<HICON> VolumeIconset() = 0;

    /// <summary>Loads and instantiates the Volume slider.</summary>
    virtual SliderComponent *VolumeSlider() = 0;

    /// <summary>Loads and instantiates the Mute OSD.</summary>
    virtual OSDComponent *MuteOSD() = 0;

    /// <summary>Loads and instantiates the Eject OSD.</summary>
    virtual OSDComponent *EjectOSD() = 0;

    /// <summary>
    /// Creates a Bitmap instance from a file on disk.
    /// </summary>
    /// <param name="fileName">Path to the image file to load.</param>
    /// <returns>
    /// Bitmap loaded from disk, or <c>nullptr</c> if the bitmap could not be
    /// loaded. Users of this method are responsible for deleting the bitmap
    /// instance when finished with it.
    /// </returns>
    static Gdiplus::Bitmap *LoadImg(std::wstring fileName);
};
