// Copyright (c) 2015, Matthew Malensek.
// Distributed under the BSD 2-Clause License (see LICENSE.txt for details)

#pragma once

#include <Windows.h>

#include "../TinyXml2/tinyxml2.h"
#include "Skin.h"
#include "SkinInfo.h"

class SliderKnob;
class Text;

struct MeterComponent;

using tinyxml2::XMLElement;

class SkinV2 : public Skin, public SkinInfo {
public:
    SkinV2(std::wstring skinXML);
    ~SkinV2();

    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();
    virtual OSDComponent *BrightnessOSD();

    virtual HICON EjectIcon();
    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();

private:
    Text *CreateText(Gdiplus::Bitmap *baseBitmap);
    SliderKnob *CreateKnob();
};
