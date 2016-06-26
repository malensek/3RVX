// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "../Meter.h"

/**
 * A bitstrip meter allows for full customizability of each meter state and
 * resembles a strip of film; each successive state image is placed after the
 * previous and then divided into discrete image "tiles." This allows full
 * control of the display for cases where the other meter types are not flexible
 * enough. For example, a bitstrip could be used to make a slideshow where each
 * change in volume, brightness, etc. shows a completely different image.
 *
 * For an live sample, see the "Ignition" skin.
 *
 * @author malensek
 */
class Bitstrip : public Meter {
public:
    Bitstrip(std::wstring bitmapName, int x, int y, int units);
    virtual void Draw(Gdiplus::Bitmap *buffer, Gdiplus::Graphics *graphics);
};