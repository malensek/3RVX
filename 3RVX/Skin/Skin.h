// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

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

    /// <summary>Retrieves the iconset used for the eject icon.</summary>
    virtual std::vector<HICON> EjectIconset() = 0;

    /// <summary>Loads and instantiates the Brightness OSD.</summary>
    virtual OSDComponent *BrightnessOSD() = 0;

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

    /// <summary>
    /// Loads an iconset (represented as a vector of HICONs) from a directory.
    /// In 3RVX, iconset ordering is defined by file names; 0.ico is the first
    /// icon, 1.ico is the second, and so on.
    /// </summary>
    /// <param name="iconDir">Name of the directory to scan for icons</param>
    /// <returns>
    /// An iconset, represented as a vector of HICON instances. Note that these
    /// icons will have to be cleaned up when they are no longer needed (use the
    /// DestroyIcon() function).
    /// </returns>
    static std::vector<HICON> ReadIconDirectory(std::wstring iconDir);
};
