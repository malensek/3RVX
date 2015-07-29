#pragma once

#include <Windows.h>

#include "Skin.h"

class MultiSkin : public Skin {
public:
    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();

    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();

};