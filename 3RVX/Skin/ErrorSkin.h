// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include "Skin.h"
#include "SkinInfo.h"

class Meter;
class SliderKnob;
class SoundPlayer;

struct MeterComponent;

using tinyxml2::XMLElement;

/// <summary>
/// This skin implementation serves as a last resort in the skin loader
/// pipeline. If no suitable resources have been found in any of the other skins
/// in the pipeline, then the ErrorSkin displays an error message to let the
/// user know something went wrong.
/// </summary>
class ErrorSkin : public Skin {
public:
    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();
    virtual OSDComponent *BrightnessOSD();

    virtual std::vector<HICON> EjectIconset();
    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();
};