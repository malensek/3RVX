#pragma once

#include <Windows.h>

#include "Skin.h"
#include "SkinInfo.h"

class SkinV2 : public Skin, public SkinInfo {
public:
    SkinV2(std::wstring skinXML);
    ~SkinV2();

    virtual OSDComponent *VolumeOSD();
    virtual OSDComponent *MuteOSD();
    virtual OSDComponent *EjectOSD();

    virtual std::vector<HICON> VolumeIconset();

    virtual SliderComponent *VolumeSlider();

private:
    OSDComponent *CreateOSDComponent(char *osdType);

};
