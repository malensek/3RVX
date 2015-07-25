#pragma once

#pragma comment(lib, "gdiplus.lib")

#include <Windows.h>
#include <gdiplus.h>
#include <vector>

#include "../MeterWnd/Meter.h"

class OSDSkin {
public:
    OSDSkin(Gdiplus::Bitmap *background, Gdiplus::Bitmap *mask,
        std::vector<Meter *> meters, std::vector<HICON> iconset) :
    background(background),
    mask(mask),
    meters(meters),
    iconset(iconset) {

    }

    const Gdiplus::Bitmap *background;
    const Gdiplus::Bitmap *mask;
    const std::vector<Meter *> meters;
    const std::vector<HICON> iconset;
};